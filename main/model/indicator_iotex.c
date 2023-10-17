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
#include "iotex_event.h"

// #include "wsiotsdk.h"

static const char *TAG = "IoTex-model";

static W3B_CFG w3b_cfg;
static bool    bind_flag;

static void __cfg_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case VIEW_EVENT_MQTT_IOTEX_CFG: { // trigger to read new cfg
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_CFG");

            int len = sizeof(W3B_CFG);
            if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
                ESP_LOGE(TAG, "cfg_read_fn failed");
                return;
            }
            break;
        }
        case VIEW_EVENT_MQTT_IOTEX_BINDING: { // tirgger to read new bind_flag
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_BINDING");
            bind_flag_read_fn(&bind_flag);
            break;
        }
        default:
            break;
    }
    return;
}
static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case VIEW_EVENT_IOTEX_USER_CONFIRM: { // 用户按下 Confirm 的触发，无需解析数据
            ESP_LOGI(TAG, "event: VIEW_EVENT_IOTEX_USER_CONFIRM");


            break;
        }
        default:
            break;
    }
    return;
}

/**
 * @brief 显示到屏幕上，传递 w3b_cfg_interface 即可 
 * @param w3b_cfg_interface
 * esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_WRITE, &w3b_cfg.cfg, sizeof(w3b_cfg_interface), portMAX_DELAY);
 */

int indicator_iotex_init(void)
{

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                                                             __cfg_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_BINDING,
                                                             __cfg_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_IOTEX_USER_CONFIRM,
                                                             __view_event_handler, NULL, NULL));


}