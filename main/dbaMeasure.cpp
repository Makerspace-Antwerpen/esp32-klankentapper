#include "dbaMeasure.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include <math.h>
#include "iir-filter.hpp"

#define SAMPLE_RATE 48000
#define SAMPLE_BITS 32    // bits
#define SAMPLE_T int32_t
#define SAMPLE_CONVERT(s) (s >> (SAMPLE_BITS - MIC_BITS))
#define SAMPLES_SHORT (SAMPLE_RATE / 8) // ~125ms
//#define SAMPLES_SHORT SAMPLE_RATE
#define SAMPLES_LEQ (SAMPLE_RATE * LEQ_PERIOD)
#define DMA_BANK_SIZE (SAMPLES_SHORT / 16)
#define DMA_BANKS 32

#define MIC_OFFSET_DB     0    // Default offset (sine-wave RMS vs. dBFS)
#define MIC_REF_DB        90.0      // dB(SPL)
#define MIC_BITS          24        // valid bits in I2S data
#define MIC_REF_AMPL      182571    // Amplitude at 94dB(SPL) (-26dBFS from datasheet, i.e. (2^23-1)*10^(-26/20) )
#define MIC_OVERLOAD_DB   120.0     // dB - Acoustic overload point*/
#define MIC_NOISE_DB      30        // dB - Noise floor*/



void DbaMeasure::audioProcessTask(void *parameter)
{
    const double A_weighting_B[] = {0.169994948147430, 0.280415310498794, -1.120574766348363, 0.131562559965936, 0.974153561246036, -0.282740857326553, -0.152810756202003};
    const double A_weighting_A[] = {1.0, -2.12979364760736134, 0.42996125885751674, 1.62132698199721426, -0.96669962900852902, 0.00121015844426781, 0.04400300696788968};
    const double Mic_Flattening_B[] = {1.001240684967527, -1.996936108836337, 0.995703101823006};
    const double Mic_Flattening_A[] = {1.0, -1.997675693595542, 0.997677044195563};

    IIR_FILTER A_weighting(A_weighting_B, A_weighting_A);
    IIR_FILTER Mic_Flattening(Mic_Flattening_B, Mic_Flattening_A);


    uint16_t arraySize;
    arraySize = SAMPLES_SHORT * sizeof(int32_t);
    int32_t *audioInBuffer;
    audioInBuffer = (int32_t *) malloc(arraySize);
    uint32_t bytes_read;
    uint32_t bytes_written;
    float average = 0;

    this->dBaQueue = xQueueCreate(20, sizeof(double));

    while (1)
    {
        IIR_ACCU_T sum_sqr_weighted = 0;
        IIR_BASE_T s;

        esp_err_t err = i2s_read(I2S_NUM_0, audioInBuffer, (arraySize), &bytes_read, 1000);
        vTaskDelay(1);
        //printf("firstNumber: %d  ", audioInBuffer[0]);




        IIR_ACCU_T total = 0;
        for (uint16_t i = 0; i < SAMPLES_SHORT; i++)
        {
            s = SAMPLE_CONVERT(audioInBuffer[i]);

            s = Mic_Flattening.filter(s);

            s = A_weighting.filter(s);

            ACCU_SQR(sum_sqr_weighted, s);
        }

        double squaredTotal = sqrt(double(sum_sqr_weighted)/SAMPLES_SHORT);
        // Calculate dB values relative to MIC_REF_AMPL and adjust for reference dB
        double short_SPL_dB = MIC_OFFSET_DB + MIC_REF_DB + 20 * log10(sqrt(double(sum_sqr_weighted) / SAMPLES_SHORT) / MIC_REF_AMPL);

        


        // printf("db: %lf\n", short_SPL_dB);
        xQueueSend(this->dBaQueue, &short_SPL_dB, (TickType_t) 10);

    }
}

DbaMeasure::DbaMeasure()
{
    // CONFIG OF I2S_0 per
    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        //.communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = DMA_BANKS,
        .dma_buf_len = DMA_BANK_SIZE,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0};

    
    if (ESP_OK != i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL))
    {
        printf("i2s_driver_install: error\n");
    }

    i2s_pin_config_t pin_config = {
        .bck_io_num = -1, // Bit Clock.
        .ws_io_num = 18,  // Word Select aka left/right clock aka LRCL. !!! When in pdm mode this is the clock
        .data_out_num = -1,
        .data_in_num = 17, // Data-out of the mic.
    };

    if (ESP_OK != i2s_set_pin(I2S_NUM_0, &pin_config))
    {
        printf("i2s_set_pin: error\n");
    }


    // INIT audio processing thread
    xTaskCreate(
        this->audioProcessWrapper,
        "task",
        2048,
        this,
        1,
        NULL
    );

    
    
}

void DbaMeasure::audioProcessWrapper(void * obj){
    DbaMeasure * m = (DbaMeasure *) obj;
    m->audioProcessTask(NULL);
}

