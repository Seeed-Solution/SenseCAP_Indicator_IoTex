

#ifndef CONFIG_H
#define CONFIG_H

#include "bsp_board.h"
#include "esp_err.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "esp_system.h"
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(VIEW_EVENT_BASE);
extern esp_event_loop_handle_t view_event_handle;

ESP_EVENT_DECLARE_BASE(CFG_EVENT_BASE);
extern esp_event_loop_handle_t cfg_event_handle;

ESP_EVENT_DECLARE_BASE(MQTT_APP_EVENT_BASE);
extern esp_event_loop_handle_t mqtt_app_event_handle;

ESP_EVENT_DECLARE_BASE(DATA_EVENT_BASE);
extern esp_event_loop_handle_t data_event_handle;

#define SN_MAX_LEN      18
#define WALLET_MAX_LEN  160

enum W3B_CFG_EVENT{
    CFG_EVENT_READ,
    CFG_EVENT_WRITE,
    CFG_EVENT_VIEW,
    BIND_EVENT_READ,
    BIND_EVENT_WRITE,
    BIND_EVENT_VIEW,
    BIND_EVENT_ALL,
    W3B_CFG_EVENT_ALL,
};

enum MQTT_APP_EVENT{
    MQTT_APP_START,
    MQTT_APP_RESTART,
    MQTT_APP_STOP,
    MQTT_APP_ALL,
};

typedef struct {
    char             sn[SN_MAX_LEN + 1];
    int              sn_len;
    char             wallet[WALLET_MAX_LEN + 1];
    int              wallet_len;
} w3b_cfg_interface;

typedef enum {
    CFG_OK,
    CFG_FAIL,
}CFG_STATUS;

enum SENSOR_DATA_EVENT{
    DATA_EVENT_SRNSOR_CAPTURE,
    DATA_EVENT_SRNSOR_UPLOAD,
    DATA_EVENT_ALL,
};

void response_cmd(CFG_STATUS status, char *resp);
esp_err_t w3b_cfg_init();
#ifdef __cplusplus
}
#endif

#endif
