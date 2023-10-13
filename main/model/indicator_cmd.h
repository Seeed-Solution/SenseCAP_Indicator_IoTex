#ifndef indicator_CMD_H
#define indicator_CMD_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "indicator_model.h"


int indicator_cmd_init(void);
void response_cmd(CFG_STATUS status, char *resp);
esp_err_t w3b_cfg_init();

#endif