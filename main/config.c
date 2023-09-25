/**
 * @file config.c
 * @date  18 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */
#include "config.h"
#include "esp_mac.h"
#include "freertos/semphr.h"
#include "indicator_storage.h"
#include "nvs.h"

static const char *TAG = "W3B_CFG";

#define IOTEX_CFG_STORAGE     "IoTexCFG"
#define IOTEX_BINDING_STORAGE "IoTexBindFlag"

typedef struct W3B_CFG {
    uint8_t           mac[6];
    w3b_cfg_interface cfg;
} W3B_CFG;

esp_err_t bind_flag_write_fn(bool *flag);
esp_err_t bind_flag_read_fn(bool *flag);
esp_err_t cfg_write_fn(W3B_CFG *cfg);
esp_err_t cfg_read_fn(W3B_CFG *cfg, int *len);
void      response_cmd(CFG_STATUS status, char *resp);

W3B_CFG w3b_cfg;
bool    IoTexBindingFlag = false;

static void _event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case CFG_EVENT_READ: {
            ESP_LOGI(TAG, "event: CFG_EVENT_READ");

            int len = sizeof(W3B_CFG);
            if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
                ESP_LOGE(TAG, "cfg_read_fn failed");
                response_cmd(CFG_FAIL, "Read failed");
                return;
            }
            char buf[526];
            sprintf(buf, "CMD_RESP:%s," MACSTR ",%s,%s\r\n", "OK", MAC2STR(w3b_cfg.mac), w3b_cfg.cfg.sn, w3b_cfg.cfg.wallet);
            printf("%s", buf);
            break;
        }
        case BIND_EVENT_READ: {
            ESP_LOGI(TAG, "event: BIND_EVENT_READ");
            if (bind_flag_read_fn(&IoTexBindingFlag) != ESP_OK) {
                ESP_LOGE(TAG, "bind_flag_read_fn failed");
                return;
            }
            break;
        }
        case CFG_EVENT_WRITE: {
            w3b_cfg_interface *rev_cfg = (w3b_cfg_interface *)event_data;

            ESP_LOGI(TAG, "event: CFG_EVENT_WRITE");
            memcpy(&w3b_cfg.cfg, rev_cfg, sizeof(w3b_cfg_interface));

            if (cfg_write_fn(&w3b_cfg) != ESP_OK) {
                ESP_LOGE(TAG, "cfg_write_fn failed");
                response_cmd(CFG_FAIL, "Write failed");
                return;
            }
            response_cmd(CFG_OK, NULL);
            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_VIEW,
                              NULL, 0, portMAX_DELAY);
            break;
        }
        case BIND_EVENT_WRITE: {
            ESP_LOGI(TAG, "event: BIND_EVENT_WRITE");
            bool *rev_flag   = (bool *)event_data;
            IoTexBindingFlag = *rev_flag;
            if (bind_flag_write_fn(&IoTexBindingFlag) != ESP_OK) {
                ESP_LOGE(TAG, "bind_flag_write_fn failed");
                // response_cmd(CFG_FAIL, "Write failed");
                return;
            }
            ESP_LOGI(TAG, "IoTexBindingFlag:%d", IoTexBindingFlag);
            // response_cmd(CFG_OK, NULL);
            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_VIEW,
                              NULL, NULL,
                              portMAX_DELAY);
            break;
        }
        case CFG_EVENT_VIEW: {
            ESP_LOGI(TAG, "event: CFG_EVENT_VIEW");
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                              &w3b_cfg.cfg, sizeof(w3b_cfg.cfg),
                              portMAX_DELAY);
            break;
        }
        case BIND_EVENT_VIEW: {
            ESP_LOGI(TAG, "event: BIND_EVENT_VIEW");
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_BINDING,
                              &IoTexBindingFlag, sizeof(IoTexBindingFlag),
                              portMAX_DELAY);
            break;
        }
        default:
            break;
    }
}

esp_err_t w3b_cfg_init()
{

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_READ,
                                                             _event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_WRITE,
                                                             _event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, BIND_EVENT_READ,
                                                             _event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, BIND_EVENT_WRITE,
                                                             _event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_VIEW,
                                                             _event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, BIND_EVENT_VIEW,
                                                             _event_handler, NULL, NULL));

    int len       = sizeof(W3B_CFG);

    esp_err_t err = cfg_read_fn(&w3b_cfg, &len);
    if (err == ESP_OK && len == sizeof(W3B_CFG)) {
        ESP_LOGI(TAG, "W3B cfg read successful");
    } else {
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGI(TAG, "W3B cfg not find");
        } else {
            ESP_LOGI(TAG, "W3B cfg read err:%d", err);
        }
        err = esp_read_mac(w3b_cfg.mac, ESP_MAC_WIFI_STA);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_read_mac failed");
            return ESP_ERR_NOT_FOUND;
        }
        w3b_cfg.cfg.sn_len     = SN_MAX_LEN;
        w3b_cfg.cfg.wallet_len = WALLET_MAX_LEN;
        w3b_cfg.cfg.sn[0]      = '\0';
        w3b_cfg.cfg.wallet[0]  = '\0';
    }
    cfg_write_fn(&w3b_cfg);
    ESP_LOGI(TAG, "w3b_cfg_init success");
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_VIEW,
                      NULL, 0, portMAX_DELAY);
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_VIEW,
                      NULL, 0, portMAX_DELAY);
}

esp_err_t bind_flag_write_fn(bool *flag)
{
    return indicator_storage_write(IOTEX_BINDING_STORAGE, (void *)flag, sizeof(bool));
}

esp_err_t bind_flag_read_fn(bool *flag)
{
    int len;
    return indicator_storage_read(IOTEX_BINDING_STORAGE, (void *)flag, &len);
}

esp_err_t cfg_write_fn(W3B_CFG *cfg)
{
    return indicator_storage_write(IOTEX_CFG_STORAGE, (void *)cfg, sizeof(W3B_CFG));
}
esp_err_t cfg_read_fn(W3B_CFG *cfg, int *len)
{
    return indicator_storage_read(IOTEX_CFG_STORAGE, (void *)cfg, len);
}

#define EnumToStr(x) #x
void response_cmd(CFG_STATUS status, char *resp)
{
    static const char *OK = "OK";
    static const char *FAIL = "FAIL";
    switch (status) {
        case CFG_OK:
            printf("CMD_RESP:%s\r\n", OK);
            break;
        case CFG_FAIL:
            if (resp == NULL)
                resp = "\0";
            printf("CMD_RESP:%s,%s\r\n", FAIL, resp);
            break;
        default:
            break;
    }
}
#undef EnumToStr