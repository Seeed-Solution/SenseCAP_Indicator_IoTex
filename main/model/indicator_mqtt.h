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

#include "cJSON.h"
#include "indicator_view.h"
// #include "portal_decoder.h"
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
typedef struct instance_mqtt_t {
    // bool net_flag; // organize by the father MQTT
    char                     *mqtt_name;
    bool                      mqtt_connected_flag;
    esp_mqtt_client_handle_t  mqtt_client; // point
    esp_mqtt_client_config_t *mqtt_cfg;
    esp_event_handler_t       mqtt_event_handler;
    MQTTStartFn               mqtt_starter;
    bool is_using;
} instance_mqtt_t;

struct instance_mqtt_t;

// Variables

extern bool            mqtt_net_flag;
// extern instance_mqtt_t mqtt_portal_instance;
// extern instance_mqtt_t mqtt_jetson_instance;

//////////////////////////////////////////

// Second set of macros and data structures
#define MQTT_PLATFORM   \
    X(Web3Stream, "Web3Stream") \

#define X(name, str) name,
typedef enum MQTT_PLATFORM_ENUM {
    MQTT_PLATFORM
        MQTT_PLATFORM_MAX
} MQTT_PLATFORM_ENUM;
#undef X

#define X(name, str) str,
extern const char *MQTT_PLATFORM_STR[];
#undef X

//////////////////////////////////////////
void log_error_if_nonzero(const char *message, int error_code);

int  indicator_mqtt_init(void);
void mqtt_start_interface(instance_mqtt_t *instance, enum MQTT_START_ENUM flag);
#endif // indicator_mqtt_H
