/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/i2s.h"
#include "driver/gpio.h"
#include "math.h"

#define SAMPLE_RATE 48000
#define SAMPLE_BITS 16    // bits
#define SAMPLE_T int32_t
#define SAMPLE_CONVERT(s) (s >> (SAMPLE_BITS - MIC_BITS))
#define SAMPLES_SHORT (SAMPLE_RATE / 8) // ~125ms
//#define SAMPLES_SHORT 1024
#define SAMPLES_LEQ (SAMPLE_RATE * LEQ_PERIOD)
#define DMA_BANK_SIZE (SAMPLES_SHORT / 16)
#define DMA_BANKS 32

void audioProcessTask(void *parameter) {
    uint16_t arraySize;
    arraySize = 6000 * 8;
    int32_t *buffer16;
    buffer16 = (int32_t *) malloc(arraySize);
    uint32_t bytes_read;
    uint32_t bytes_written;
    float average = 0;
    while (1)
    {
        esp_err_t err = i2s_read(I2S_NUM_0, buffer16, (arraySize), &bytes_read, 1000);
        vTaskDelay(1);
        //printf("firstNumber: %d  ", buffer16[0]);
        uint64_t total = 0;
        for (uint16_t i = 0; i < 6000; i++)
        {
            total += buffer16[i];
        }
        //printf("total: %llu  ", total);
        int32_t mean = (int32_t) total / 6000;

        long double squaredTotal = 0;

        for (uint16_t i = 0; i < 6000; i++)
        {
            uint32_t dc_corrected = buffer16[i];// - mean;
            squaredTotal += pow(buffer16[i],2);

        }

        


        printf("squaredTotal: %Lf\n", squaredTotal);

    }
    
}

void app_main(void)
{
    // CONFIG OF I2S_0 per
    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
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
        audioProcessTask,
        "task",
        2048,
        NULL,
        1,
        NULL
    );
}
