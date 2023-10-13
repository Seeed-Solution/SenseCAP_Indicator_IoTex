#ifndef INDICATOR_MQTT_IOTEX_H
#define INDICATOR_MQTT_IOTEX_H

#include "indicator_model.h"
#include "indicator_model.h"

struct upload_data {
    int valid_bit;
    int co2;
    int tvoc;
    int temp;
    int humidity;
};

enum upload_data_type {
    UPLOAD_DATA_CO2,
    UPLOAD_DATA_TVOC,
    UPLOAD_DATA_TEMP,
    UPLOAD_DATA_HUMIDITY,
};

#define BIT_UPLOAD_DATA_CO2         0x1
#define BIT_UPLOAD_DATA_TVOC        0x2
#define BIT_UPLOAD_DATA_TEMP        0x4
#define BIT_UPLOAD_DATA_HUMIDITY    0x8
#define BIT_UPLOAD_DATA_MASK        (BIT_UPLOAD_DATA_CO2 | BIT_UPLOAD_DATA_TVOC | BIT_UPLOAD_DATA_TEMP | BIT_UPLOAD_DATA_HUMIDITY)
#define BIT_UPLOAD_DATA_READY       BIT_UPLOAD_DATA_MASK

void iotex_upload_data_set_value(int value, enum upload_data_type data_type);

#endif /*INDICATOR_MQTT_IOTEX_H*/