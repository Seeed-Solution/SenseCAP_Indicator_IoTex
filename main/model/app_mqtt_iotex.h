/**
 * @file indicator_mqtt_iotex.h
 * @date  18 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef INDICATOR_MQTT_IOTEX_H
#define INDICATOR_MQTT_IOTEX_H
#include "indicator_mqtt.h"

#define KEY_LEN 20
// typedef struct jetson_data {
//     char                key[KEY_LEN];         // 11 bytes
//     int16_t             value;                // 2 bytes
//     uint32_t            lastUpdatedTimestamp; // 4 bytes
//     int                 order;                // x,0 | x,1
//     struct jetson_data *next;                 // 4 bytes
//     struct jetson_data *prev;                 // 4 bytes
// } jetson_data_t;

#define IOTEX_LOG_ENABLE 1
void mqtt_iotex_init(void);
#endif /*INDICATOR_MQTT_IOTEX_H*/