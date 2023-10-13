/**
 * @file indicator_iotex.c
 * @date  12 October 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "indicator_cmd.h"
// #include "wsiotsdk.h"

#include "model_data.h"
#include "view_data.h"

static const char *TAG = "IoTex-model";

typedef struct {
    bool              bind_flag;
    w3b_cfg_interface cfg;
} dev_info_c;

enum SERVER_STATUS {
    IOTEX_STATUS_NO_NETWORK       = 0,
    IOTEX_STATUS_PORTAL_REGIESTER = 1,
    IOTEX_STATUS_DEVICE_BINDING   = 2,
    IOTEX_STATUS_REGISTER_SUCCESS = 3,
};

static TaskHandle_t xTaskHandle_iotex = NULL;
static bool         net_flag          = false;

extern enum SERVER_STATUS get_status_from_server();
extern esp_mqtt_client_handle_t mqtt_client;

void iotex_thread_entry(void *p_arg)
{
    ESP_LOGI(TAG, "iotex_thread_entry start...");

    enum SERVER_STATUS previous_status = IOTEX_STATUS_NO_NETWORK;

    // 获取当前注册的信息
    dev_info_c *info                   = (dev_info_c *)p_arg;
    bool        bind_flag              = info->bind_flag;

    w3b_cfg_interface cfg;
    memcpy(&cfg, &info->cfg, sizeof(w3b_cfg_interface));

    // display the members of the config structure
    ESP_LOGI(TAG, "bind_flag: %d", bind_flag);
    ESP_LOGI(TAG, "cfg.sn: %s", cfg.sn);
    ESP_LOGI(TAG, "cfg.wallet: %s", cfg.wallet);

    // 如果 MQTT 服务 启动了，则关闭
    // if (mqtt_client != NULL) {
    //     esp_mqtt_client_stop(mqtt_client);
    //     esp_mqtt_client_destroy(mqtt_client);
    //     mqtt_client = NULL;
    // }

    while (1) {
        // if (!net_flag) { // 如果突然断开了
        //     vTaskDelay(pdMS_TO_TICKS(1000));
        //     continue;
        // }
        enum SERVER_STATUS cur_status = get_status_from_server();
        // enum SERVER_STATUS cur_status = 0;

        switch (cur_status) {
            case IOTEX_STATUS_NO_NETWORK:
                break;
            case IOTEX_STATUS_PORTAL_REGIESTER:
                if (bind_flag) // 用户还未在 Portal 上注册
                {
                    bind_flag = false;
                    ESP_LOGI(TAG, "turn to portal: bind_flag: %d", bind_flag);
                    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &bind_flag, sizeof(bool), portMAX_DELAY);
                }
                ESP_LOGI(TAG, "IOTEX_STATUS_PORTAL_REGIESTER");
                // TODO change screen EVENT Tell people to register on the portal
                enum INDICATOR_PAGE turnTOpage = INDICATOR_PAGE_PORTAL_REGIESTER_PAGE;
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_IOTEX_CONTROL, &turnTOpage, sizeof(enum INDICATOR_PAGE), portMAX_DELAY);
                break;
            case IOTEX_STATUS_DEVICE_BINDING:
                // Portal 还在注册中，需要等待
                ESP_LOGI(TAG, "IOTEX_STATUS_DEVICE_REGIESTER ING...");
                break;
            case IOTEX_STATUS_REGISTER_SUCCESS:
                if ((cur_status ^ previous_status) != IOTEX_STATUS_REGISTER_SUCCESS) { // 1/2 -> 3
                    // TODO interaction: successful registration Pop
                    // jump to screen_data
                    ESP_LOGI(TAG, "IOTEX_STATUS_REGISTER_SUCCESS");
                    enum INDICATOR_PAGE turnTOpage = INDICATOR_PAGE_PORTAL_REGIESTER_SUCCESS;
                    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_IOTEX_CONTROL, &turnTOpage, sizeof(enum INDICATOR_PAGE), portMAX_DELAY);
                }
                if (!bind_flag) {
                    bind_flag = true;
                    ESP_LOGI(TAG, "bind successful");
                    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &bind_flag, sizeof(bool), portMAX_DELAY);
                }

                // if(local_server_started == false)
                // {
                //     run_local_mqtt_server();
                // }

                break;
            default:
                break;
        }
        previous_status = cur_status;
        vTaskDelay(10000 / portTICK_PERIOD_MS);

        // if (city_flag && time_zone_flag) {
        //     break;
        // }
    }
    vTaskDelete(NULL);
}

static void __event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    static dev_info_c dev_info;
    switch (id) {
        case VIEW_EVENT_MQTT_IOTEX_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_CFG");
            w3b_cfg_interface *p_cfg = (w3b_cfg_interface *)event_data;
            memcpy(&dev_info.cfg, p_cfg, sizeof(w3b_cfg_interface));
            goto entry;
            break;
        }
        case VIEW_EVENT_MQTT_IOTEX_BINDING: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_BINDING");
            bool *p_flag       = (bool *)event_data;
            dev_info.bind_flag = *p_flag;
            break;
        }
        case VIEW_EVENT_WIFI_ST:
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            struct view_data_wifi_st *p_st = (struct view_data_wifi_st *)event_data;
            if (p_st->is_network && !net_flag) {
                net_flag = true;
                goto entry;
            } else {
                net_flag = false;
            }
            break;
        default:
            return;
            break;
    }
entry:
    if (xTaskHandle_iotex != NULL) {
        vTaskDelete(xTaskHandle_iotex);
    }
    xTaskCreate(iotex_thread_entry, "iotex_thread_entry", 1024 * 10, &dev_info, 5, &xTaskHandle_iotex);
    return;
}


int indicator_iotex_init(void)
{


    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_WIFI_ST,
                                                             __event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                                                             __event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_BINDING,
                                                             __event_handler, NULL, NULL));


}