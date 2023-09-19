

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

#define SN_MAX_LEN      18
#define WALLET_MAX_LEN  160

enum CFG_EVENT{
    CFG_EVENT_READ,
    CFG_EVENT_WRITE,
    CFG_EVENT_ALL,
};

typedef struct {
    char             sn[SN_MAX_LEN + 1];
    int              sn_len;
    char             wallet[WALLET_MAX_LEN + 1];
    int              wallet_len;
} w3b_cfg_interface;

enum at_state {
    OK,
    ERR,
};

void response_cmd(enum at_state state, char *resp);
esp_err_t w3b_cfg_init();
#ifdef __cplusplus
}
#endif

#endif
