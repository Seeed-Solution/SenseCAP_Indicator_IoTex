/**
 * @file model_data.c
 * @author Spencer
 * @brief
 * 无法主动读取信息，仅能通过 event 去做级联而传递出去。
 * xxx_READ 和 xxx_WRITE 都是为 cmd 的 CI 控制提供的；
 * 每个 WRITE 都会触发 VIEW 的显示；即 xxx_VIEW 与 view_event_handle 相关，直接 trigger 视图显示
 *
 * @version 0.1
 * @date 2023-10-16
 */

#include "model_data.h"
#include "esp_mac.h"
#include "freertos/semphr.h"
#include "indicator_storage.h"
#include "nvs.h"
#define IOTEX_CFG_STORAGE     "IoTexCFG"
#define IOTEX_BINDING_STORAGE "IoTexBindFlag"
#define IOTEX_ETH_STORAGE     "IoTexETH"

static const char *TAG = "model_data";

esp_err_t bind_flag_write_fn(bool *flag);
esp_err_t bind_flag_read_fn(bool *flag);
esp_err_t cfg_write_fn(W3B_CFG *cfg);
esp_err_t cfg_read_fn(W3B_CFG *cfg, int *len);

W3B_CFG        w3b_cfg;
bool           IoTexBindingFlag = false;
extern eth_cfg eth_address;
/**
 * @brief xx_VIEW 与 view_event_handle 相关，直接 trigger 视图显示
 *
 * @param handler_args
 * @param base
 * @param id
 * @param event_data
 */
static void _cfg_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        // case CFG_EVENT_READ: { // for host to read SN and // wallet
        //     ESP_LOGI(TAG, "event: CFG_EVENT_READ");

        //     // int len = sizeof(W3B_CFG);
        //     // if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
        //     //     ESP_LOGE(TAG, "cfg_read_fn failed");
        //     //     response_cmd(CFG_FAIL, "Read failed");
        //     //     return;
        //     // }
        //     // char buf[526];
        //     // sprintf(buf, "CMD_RESP:%s," MACSTR ",%s,%s\r\n", "OK", MAC2STR(w3b_cfg.mac), w3b_cfg.cfg.sn, w3b_cfg.cfg.wallet);
        //     // printf("%s", buf);
        //     break;
        // }
        // case BIND_EVENT_READ: { // TODO: NOTHING to do
        //     ESP_LOGI(TAG, "event: BIND_EVENT_READ");
        //     // if (bind_flag_read_fn(&IoTexBindingFlag) != ESP_OK) {
        //     //     ESP_LOGE(TAG, "bind_flag_read_fn failed");
        //     //     return;
        //     // }
        //     break;
        // }
        case DEVICE_ETH_EVENT_TRIGGER: {
            // display
            ESP_LOGI(TAG, "event: DEVICE_ETH_EVENT_TRIGGER");
            // 获取 string
            eth_cfg *rev_cfg = (eth_cfg *)event_data;
            if (rev_cfg == NULL)
                return;
            // save for upload to host
            memcpy(&eth_address, rev_cfg, sizeof(eth_cfg));
            if (eth_addr_write_fn(&eth_address) != ESP_OK) {
                ESP_LOGE(TAG, "eth_addr_write_fn failed");
                return ESP_ERR_NOT_FOUND;
            }
            break;
            // esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_DEVICE_ETH_ADDRESS,
            //                   &rev_cfg, sizeof(eth_cfg),
            //                   portMAX_DELAY);
            // 不用顯示，上位機進行顯示
        }
        case CFG_EVENT_WRITE: {
            w3b_cfg_interface *rev_cfg = (w3b_cfg_interface *)event_data;

            ESP_LOGI(TAG, "event: CFG_EVENT_WRITE");
            if (rev_cfg->sn_len > 0) {
                w3b_cfg.cfg.sn_len = rev_cfg->sn_len;
                memcpy(&w3b_cfg.cfg.sn, rev_cfg->sn, sizeof(rev_cfg->sn));
            } else {
                // memset(&w3b_cfg.cfg.sn, 0, sizeof(w3b_cfg.cfg.sn));
                // w3b_cfg.cfg.sn_len = rev_cfg->sn_len;
                // 不覆盖，所以comments
            }
            if (rev_cfg->wallet_len > 0) {
                w3b_cfg.cfg.wallet_len = rev_cfg->wallet_len;
                memcpy(&w3b_cfg.cfg.wallet, rev_cfg->wallet, sizeof(rev_cfg->wallet));
            } else {
                // memset(&w3b_cfg.cfg.wallet, 0, sizeof(w3b_cfg.cfg.wallet));
                // w3b_cfg.cfg.wallet_len = rev_cfg->wallet_len;
                // 不覆盖，所以comments，使用之前獲取的内容
            }
            // memcpy(&w3b_cfg.cfg, rev_cfg, sizeof(w3b_cfg_interface));

            if (cfg_write_fn(&w3b_cfg) != ESP_OK) {
                ESP_LOGE(TAG, "cfg_write_fn failed");
                response_cmd(CFG_FAIL, "Write failed");
                return;
            }
            response_cmd(CFG_OK, NULL);
            /* View and trigger MQTT to restart once send w3b_cfg */
            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_VIEW,
                              NULL, 0, portMAX_DELAY);
            break;
        }
        case BIND_EVENT_WRITE: { // From VIEW widget
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
        default:
            break;
    }
}

static void _view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case CFG_EVENT_VIEW: {
            ESP_LOGI(TAG, "event: CFG_EVENT_VIEW");
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                              &w3b_cfg.cfg, sizeof(w3b_cfg.cfg),
                              portMAX_DELAY);
            break;
        }
        case BIND_EVENT_VIEW: { /* 只與視圖相關 */
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

/**
 * @brief 中间层 cfg_event_handle 传递的信息，进行 CFG 的读取和写入，并传递给 view_event_handle 进行显示；
 *
 * @return esp_err_t
 */
esp_err_t w3b_cfg_init()
{

    // ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
    //                                                          CFG_EVENT_BASE, CFG_EVENT_READ,
    //                                                          _cfg_event_handler, NULL, NULL));

    // ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
    //                                                          CFG_EVENT_BASE, BIND_EVENT_READ,
    //                                                          _cfg_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, DEVICE_ETH_EVENT_TRIGGER,
                                                             _cfg_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_WRITE,
                                                             _cfg_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, BIND_EVENT_WRITE,
                                                             _cfg_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, CFG_EVENT_VIEW,
                                                             _view_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle,
                                                             CFG_EVENT_BASE, BIND_EVENT_VIEW,
                                                             _view_event_handler, NULL, NULL));

    /* NVS 为空，填充空值 */
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
        // w3b_cfg.cfg.sn_len     = SN_MAX_LEN;
        // w3b_cfg.cfg.wallet_len = WALLET_MAX_LEN;
        w3b_cfg.cfg.sn_len     = 0;
        w3b_cfg.cfg.wallet_len = 0;
        w3b_cfg.cfg.sn[0]      = '\0';
        w3b_cfg.cfg.wallet[0]  = '\0';
        cfg_write_fn(&w3b_cfg);
    }
    ESP_LOGI(TAG, "w3b_cfg_init success, sn:%s, wallet:%s", w3b_cfg.cfg.sn, w3b_cfg.cfg.wallet);
    /* 触发一次: 视图显示 NVS 的数据 */
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_VIEW,
                      NULL, 0, portMAX_DELAY);

    err = bind_flag_read_fn(&IoTexBindingFlag);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "bind_flag_read_fn success, IoTexBindingFlag:%d", IoTexBindingFlag);
    } else {
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGI(TAG, "bind_flag_read_fn not find");
        } else {
            ESP_LOGI(TAG, "bind_flag_read_fn err:%d", err);
        }
        IoTexBindingFlag = false;
        bind_flag_write_fn(&IoTexBindingFlag);
    }
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_VIEW,
                      NULL, 0, portMAX_DELAY);

    eth_cfg _eth_addr;
    if (eth_addr_read_fn(&_eth_addr, &len) != ESP_OK) {
        ESP_LOGE(TAG, "eth_addr_read_fn failed");
        // return ESP_ERR_NOT_FOUND;
    } else {
        ESP_LOGI(TAG, "eth_addr_read_fn success, eth_addr:%s", _eth_addr.eth);
        esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, DEVICE_ETH_EVENT_TRIGGER,
                          &_eth_addr, sizeof(eth_cfg),
                          portMAX_DELAY);
    }
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

esp_err_t eth_addr_read_fn(eth_cfg *cfg, int *len)
{
    return indicator_storage_read(IOTEX_ETH_STORAGE, (void *)cfg, len);
}

esp_err_t eth_addr_write_fn(eth_cfg *cfg)
{
    return indicator_storage_write(IOTEX_ETH_STORAGE, (void *)cfg, sizeof(eth_cfg));
}