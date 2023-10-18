/**
 * @file model_data.h
 * @date  12 October 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_board.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "esp_system.h"
#include <stdbool.h>
#include <string.h>

ESP_EVENT_DECLARE_BASE(DATA_EVENT_BASE);
ESP_EVENT_DECLARE_BASE(MODEL_EVENT_BASE);
extern esp_event_loop_handle_t model_event_handle;

ESP_EVENT_DECLARE_BASE(CFG_EVENT_BASE);
extern esp_event_loop_handle_t cfg_event_handle;

enum MODEL_EVENT_DEFINE {

    IOTEX_STATUS_NO_RESPONSE           = 0,
    IOTEX_STATUS_DEVICE_SHOULD_ENROLL  = 1,
    IOTEX_STATUS_DEVICE_CONFIRM_NEEDED = 2,
    IOTEX_STATUS_DEVICE_SUCCESS        = 3,


    /******* SENSOR_DATA_EVENT ********/
    DATA_EVENT_SRNSOR_CAPTURE,
    DATA_EVENT_SRNSOR_UPLOAD,
    DATA_EVENT_SRNSOR_UPSTREAM,

    /******* W3B_CFG_EVENT *******/
    /* for host to see the configuration Only*/
    CFG_EVENT_READ,
    CFG_EVENT_WRITE, // SN and Wallet are written by host
    /*** END ***/
    BIND_EVENT_READ,
    BIND_EVENT_WRITE,
    /* For View to display */
    CFG_EVENT_VIEW,
    BIND_EVENT_VIEW,
    /* For Model(IoTex) to process */
    CFG_EVENT_MODEL,
    BIND_EVENT_MODEL,

    /******* OTHER EVENT *******/
    MODEL_EVENT_IOTEX_USER_CONFIRM, // when the confirm been pressed

    MODEL_EVENT_ALL,
};

typedef enum {
    CFG_OK,
    CFG_FAIL,
} CFG_STATUS;

#define SN_MAX_LEN     18
#define WALLET_MAX_LEN 160

typedef struct {
    char sn[SN_MAX_LEN + 1];
    int  sn_len;
    char wallet[WALLET_MAX_LEN + 1];
    int  wallet_len;
} w3b_cfg_interface;

typedef struct W3B_CFG {
    uint8_t           mac[6];
    w3b_cfg_interface cfg;
} W3B_CFG; // the configurateion been stored.

esp_err_t w3b_cfg_init();
esp_err_t bind_flag_write_fn(bool *flag);
esp_err_t bind_flag_read_fn(bool *flag);
esp_err_t cfg_write_fn(W3B_CFG *cfg);
esp_err_t cfg_read_fn(W3B_CFG *cfg, int *len);

void response_cmd(CFG_STATUS status, char *resp);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MODEL_DATA_H*/