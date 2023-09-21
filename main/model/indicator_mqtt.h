/*
 * File: indicator.mqtt.h
 *
 * Created on:  05 September 2023
 *     Author: Spencer Yan
 *
 * Description: the class of MQTT
 *
 * Copyright: © 2023, Seeed Studio
 */

#ifndef indicator_mqtt_H
#define indicator_mqtt_H

#include "nvs.h"
#include "view_data.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "esp_wifi_types.h"
#include "mqtt_client.h"
#include <esp_mac.h>

#include <string.h>
#define MQTT_BROKER_STORAGE "MQTT" // 52 bytes

enum MQTT_START_ENUM {
    MQTT_START = 0,
    MQTT_RESTART
};

typedef struct {
    char topic[64];
    char data[256];
} mqtt_data_t;

typedef void (*MQTTStartFn)(struct instance_mqtt_t *instance); // Example function taking an int and returning void
typedef struct instance_mqtt {
    char                     *mqtt_name;
    bool                      mqtt_connected_flag;
    esp_mqtt_client_handle_t  mqtt_client; // point
    esp_mqtt_client_config_t *mqtt_cfg;
    esp_event_handler_t       mqtt_event_handler;
    MQTTStartFn               mqtt_starter;
    bool                      is_using;
} instance_mqtt, *instance_mqtt_t;

/* Variables */
bool get_mqtt_net_flag(void);

/* Functions */
void log_error_if_nonzero(const char *message, int error_code);
int  indicator_mqtt_init(void);

#endif // indicator_mqtt_H
