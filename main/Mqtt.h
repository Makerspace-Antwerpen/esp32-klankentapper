#ifndef MQTT_H
#define MQTT_H

#include "mqtt_client.h"

void mqtt_send(char *, int);
esp_mqtt_client_handle_t* mqtt_start(void);

#endif