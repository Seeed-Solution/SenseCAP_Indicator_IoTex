#ifndef __UPLOAD_DATA_H__
#define __UPLOAD_DATA_H__

#include "model_data.h"
#include "upload_data.h"

struct upload_data {
    int valid_bit;
    int co2;
    int tvoc;
    float temp;
    int humidity;
};

struct dev_mac {
    int valid;
    int8_t mac[6];
    int8_t mac_str[16];
};

enum upload_data_type {
    UPLOAD_DATA_CO2,
    UPLOAD_DATA_TVOC,
    UPLOAD_DATA_TEMP,
    UPLOAD_DATA_HUMIDITY,
};

enum dev_mac_type {
    DEV_MAC_TYPE_HEX,
    DEV_MAC_TYPE_STR,
};

#define BIT_UPLOAD_DATA_CO2         0x1
#define BIT_UPLOAD_DATA_TVOC        0x2
#define BIT_UPLOAD_DATA_TEMP        0x4
#define BIT_UPLOAD_DATA_HUMIDITY    0x8
#define BIT_UPLOAD_DATA_MASK        (BIT_UPLOAD_DATA_CO2 | BIT_UPLOAD_DATA_TVOC | BIT_UPLOAD_DATA_TEMP | BIT_UPLOAD_DATA_HUMIDITY)
#define BIT_UPLOAD_DATA_READY       BIT_UPLOAD_DATA_MASK

#define UPLOAD_DATA_TEST_DEV_MAC_OFFSET             0

void iotex_upload_data_set_value(int value, enum upload_data_type data_type);
int8_t *iotex_devinfo_mac_get(enum dev_mac_type);
int8_t *iotex_devinfo_dev_sn_get(void);
int iotex_devinfo_query_dev_sn(void);
void iotex_upload_data_set_status(int status);
void iotex_send_wallet_address_to_Seeed(char *buf, int32_t buf_len); 
void iotex_send_eth_address_to_Seeed(char *buf, int32_t buf_len); 

#endif