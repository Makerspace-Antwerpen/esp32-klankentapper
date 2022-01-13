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
#include "esp_log.h"

#include "wifi_manager.h"

extern "C" {
	#include "Mqtt.h"
}

static const char TAG[] = "main";

gpio_num_t LEDpin = (gpio_num_t)GPIO_NUM_26;

extern "C" {
	void app_main();
}

bool mqtt_on = false;

void blinkLED( void* parameters ) {
  while( true ) {
	gpio_pad_select_gpio( LEDpin );
	gpio_set_direction( LEDpin, GPIO_MODE_OUTPUT );
	gpio_set_level( LEDpin, 0 );
    vTaskDelay( 500 / portTICK_PERIOD_MS );
	gpio_set_level( LEDpin, 1 );
    vTaskDelay( 500 / portTICK_PERIOD_MS );
	if( mqtt_on ) {
		char * buff = (char *) malloc(50);
        int len = sprintf(buff,"{\"dBA\":\"%lf\"}",23.57);
        mqtt_send(buff, len);
        free(buff);
	}
  }
}

void callback_mqtt_start(void *pvParameter){
	ESP_LOGI(TAG, "I have a connection, let's start MQTT!");
	mqtt_start();
	mqtt_on = true;
}

void app_main(void)
{
	wifi_manager_start();
	wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &callback_mqtt_start);
	
	xTaskCreatePinnedToCore(
	    blinkLED,
	    "Blink LED",
	    2048,
	    NULL,
	    1,
	    NULL,
	    1
	);

}
