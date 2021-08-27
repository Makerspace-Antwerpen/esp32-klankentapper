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
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "dbaMeasure.hpp"
#include "MovingAverage.hpp"

DbaMeasure * dBaMeasure;
MovingAverage * movingAverage;



extern "C" void app_main(void)
{
    dBaMeasure = new DbaMeasure();
    movingAverage = new MovingAverage(40);
    while (1)
    {
        vTaskDelay(1);
        void * data;
        data = (void *) malloc(sizeof(double));
        xQueueReceive(dBaMeasure->dBaQueue, data, (TickType_t) 1000);
        double db = *(double *) data;
        movingAverage->addValue(db);
        printf("db: %lf\n", movingAverage->getLMA());
    }
    
}
