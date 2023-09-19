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

#define W3B_INFO_SN     "SN"
#define W3B_INFO_WALLET "WALLET_ADDR"
static const char *TAG = "W3B_CFG";

typedef struct W3B_CFG {
    uint8_t           mac[6];
    char              sn[SN_MAX_LEN + 1];
    int               sn_len;
    char              wallet[WALLET_MAX_LEN + 1];
    int               wallet_len;
    SemaphoreHandle_t mutex;
} W3B_CFG;


W3B_CFG w3b_cfg;

esp_err_t read_sn(char *buf, size_t *len)
{
    return indicator_storage_read(W3B_INFO_SN, (void *)buf, len);
}

esp_err_t read_wallet(char *buf, size_t *len)
{
    return indicator_storage_read(W3B_INFO_WALLET, (void *)buf, len);
}


#define EnumToStr(x) #x
void response_cmd(enum at_state state, char *resp)
{
    switch (state) {
        case OK:
            printf("CMD_RESP:%s\r\n", EnumToStr(OK));
            break;
        case ERR:
            if (resp == NULL)
                resp = "\0";
            printf("CMD_RESP:%s,%s\r\n", EnumToStr(ERR), resp);
            break;
        default:
            break;
    }
}

static void _event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case CFG_EVENT_READ: {
            // struct view_data_wifi_st *p_st = (struct view_data_wifi_st *)event_data
            ESP_LOGI(TAG, "event: CFG_EVENT_READ");
            xSemaphoreTake(w3b_cfg.mutex, portMAX_DELAY);
            char buf[526];
            sprintf(buf, "CMD_RESP:%s," MACSTR ",%s,%s\r\n", "OK", MAC2STR(w3b_cfg.mac), w3b_cfg.sn, w3b_cfg.wallet);
            printf("%s", buf);
            xSemaphoreGive(w3b_cfg.mutex);
            break;
        }
        case CFG_EVENT_WRITE: {
            w3b_cfg_interface *cfg = (w3b_cfg_interface *)event_data;
            ESP_LOGI(TAG, "event: CFG_EVENT_WRITE");
            xSemaphoreTake(w3b_cfg.mutex, portMAX_DELAY);

            w3b_cfg.sn_len     = cfg->sn_len;
            w3b_cfg.wallet_len = cfg->wallet_len;
            memcpy(w3b_cfg.sn, cfg->sn, cfg->sn_len);
            memcpy(w3b_cfg.wallet, cfg->wallet, cfg->wallet_len);

            // ESP_LOGI(TAG, "wirte sn:%s, wallet_addr:%s", w3b_cfg.sn, w3b_cfg.wallet);

            indicator_storage_write(W3B_INFO_SN, (void *)w3b_cfg.sn, w3b_cfg.sn_len);
            indicator_storage_write(W3B_INFO_WALLET, (void *)w3b_cfg.wallet, w3b_cfg.wallet_len);
            xSemaphoreGive(w3b_cfg.mutex);

            response_cmd(OK, NULL);
            break;
        }
    }
}


esp_err_t w3b_cfg_init()
{
    w3b_cfg.mutex = xSemaphoreCreateMutex();
    if (w3b_cfg.mutex == NULL) {
        ESP_LOGE(TAG, "xSemaphoreCreateMutex failed");
        return ESP_ERR_NO_MEM;
    }
    // take the mutex
    xSemaphoreTake(w3b_cfg.mutex, portMAX_DELAY);

    esp_err_t err = esp_read_mac(w3b_cfg.mac, ESP_MAC_WIFI_STA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_read_mac failed");
        return ESP_ERR_NOT_FOUND;
    }

    w3b_cfg.sn_len     = SN_MAX_LEN;
    w3b_cfg.wallet_len = WALLET_MAX_LEN;

    if (read_sn(w3b_cfg.sn, &w3b_cfg.sn_len) != ESP_OK) {
        w3b_cfg.sn[0] = '\0';
    }

    if (read_wallet(w3b_cfg.wallet, &w3b_cfg.wallet_len) != ESP_OK) {
        w3b_cfg.wallet[0] = '\0';
    }

    // give the mutex
    xSemaphoreGive(w3b_cfg.mutex);
    ESP_LOGI(TAG, "w3b_cfg_init success");
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_READ,
                                                             _event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_WRITE,
                                                             _event_handler, NULL, NULL));
}