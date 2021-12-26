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
#include "driver/gpio.h"

#include <esp_wifi.h>
#include <esp_netif.h>
#include "wifi_manager.h"

gpio_num_t pin = (gpio_num_t)GPIO_NUM_26;

extern "C" {
	void app_main();
}

void randomLED( void* parameters ) {
  while( true ) {
	gpio_pad_select_gpio( pin );
	gpio_set_direction(pin, GPIO_MODE_OUTPUT );
	gpio_set_level(pin, 0);  
    vTaskDelay( 500 / portTICK_PERIOD_MS );
	gpio_set_level(pin, 1);  
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}

void app_main(void)
{
    printf("Hello world!\n");
	
	wifi_manager_start();
	
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

	xTaskCreatePinnedToCore(
	    randomLED,
	    "Random LED",
	    1024,
	    NULL,
	    1,
	    NULL,
	    1
	  );

/*    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();*/
}
