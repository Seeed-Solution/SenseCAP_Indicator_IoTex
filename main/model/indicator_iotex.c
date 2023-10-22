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

/* 從NVS中獲取，由於變量被視圖占用 */
static void __cfg_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case CFG_EVENT_VIEW: { // 当 CFG_VIEW 更新后
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
        case VIEW_EVENT_IOTEX_USER_CONFIRM: { // 用户按下 Confirm 的触发的哪個按鈕，无需解析数据
            ESP_LOGI(TAG, "event: VIEW_EVENT_IOTEX_USER_CONFIRM");
            bool *user_choice = (bool *)event_data;
            if (*user_choice) {
                // 1. User press the `Bind` Button
                ESP_LOGI(TAG, "User press the `YES` Button");
                // 2. Tell model to send a up message
                // esp_event_post_to(mqtt_app_event_handle, MQTT_APP_EVENT_BASE, MQTT_APP_START, 0, portMAX_DELAY);
                // 会提供一个返回值：esp_err_t
                // 3. Pop up a confirm window
                // 4. change the button state BIND_EVENT_WRITE
                // see the global bind_flag
                // lv_obj_t *obj_pop = pop_up_custom("Confirm", "Please confirm the registrationhas been completed onthe portal");

            } else {
                ESP_LOGI(TAG, "User press the `NO` Button");
     
            }
            // esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &user_choice, sizeof(bool), portMAX_DELAY);
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