/* MQTT (over TCP) Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "Mqtt.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#include "wifi_manager.h"

static const char TAG[] = "mqtt";

esp_mqtt_client_handle_t mqtt_client;
mqtt_config_t* mqtt_config;

esp_mqtt_client_handle_t* mqtt_start(void)
{
	mqtt_config = wifi_manager_get_mqtt_config();
	ESP_LOGI( TAG, "Connecting to mqtt server %s", mqtt_config->server );
    esp_mqtt_client_config_t mqtt_cfg = {
<<<<<<< HEAD
        .uri = (char*)mqtt_config->server,
=======
        .uri = CONFIG_BROKER_URL,
        .username = CONFIG_THINGSBOARD_TOKEN,
>>>>>>> f3ad212 (Measurements working with thingsboard. Limmeted length of mqtt string.)
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);
	ESP_LOGI( TAG, "Connected to mqtt server %s", mqtt_config->server );
    return &mqtt_client;
}

void mqtt_send(char * message, int len){
	ESP_LOGI( TAG, "Trying to send %s to %s", message, (char*)mqtt_config->topic );
	if (mqtt_client == NULL)
        mqtt_start();
	if( mqtt_client != NULL )
		esp_mqtt_client_publish(mqtt_client, (char*)mqtt_config->topic, message, len, 0, 0);
}
