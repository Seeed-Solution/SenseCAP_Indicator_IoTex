/**
 * @file iotex_view.c
 * @date  21 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */
#include "iotex_view.h"
#include "ui_events.h" /* the functions need to be implemented */

static const char *TAG = "IOTEX_VIEW";

#include "lvgl/lvgl.h"

static void yes_btn_click_handler(lv_event_t *e);
static void no_btn_click_handler(lv_event_t *e);
lv_obj_t *pop_up_custom(char *title, char *text);

static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case VIEW_EVENT_MQTT_IOTEX_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_CFG");
            w3b_cfg_interface *p_cfg = (w3b_cfg_interface *)event_data;
            lv_textarea_set_text(ui_TextArea_SN, p_cfg->sn);
            lv_textarea_set_text(ui_TextArea_WAD, p_cfg->wallet);
            break;
        }
        case VIEW_EVENT_MQTT_IOTEX_BINDING: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_BINDING");
            bool *p_flag = (bool *)event_data;
            if (*p_flag == true) {
                ESP_LOGI(TAG, "Binding Flag is true");
                lv_obj_add_state(ui_btn_bind, LV_STATE_DISABLED);
            } else {
                ESP_LOGI(TAG, "Binding Flag is false");
                lv_obj_clear_state(ui_btn_bind, LV_STATE_DISABLED);
            }
        }
        default:
            break;
    }
    lv_port_sem_give();
}

/**
 * @brief The button `Confirm` being pressed
 * @attention This function is called by the button `Confirm` being pressed
 * @param e
 */
void fn_bind_confirm(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);

    bool bind_flag = true;
    // 1. User press the `Bind` Button
    ESP_LOGI(TAG, "fn_bind_confirm");

    // 2. Tell model to send a up message
    // esp_event_post_to(mqtt_app_event_handle, MQTT_APP_EVENT_BASE, MQTT_APP_START, 0, portMAX_DELAY);
        // 会提供一个返回值：esp_err_t
    // 3. Pop up a confirm window
    // 4. change the button state BIND_EVENT_WRITE
    lv_obj_t *obj_pop = pop_up_custom("Confirm", "Please confirm the registrationhas been completed onthe portal");    
}

int iotex_view_init(void)
{

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                                                             __view_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_BINDING,
                                                             __view_event_handler));
}

lv_obj_t *pop_up_custom(char *title, char *text)
{
    // 创建一个主屏幕
    lv_obj_t *main_screen = lv_scr_act();

    // 创建一个弹窗背景
    lv_obj_t *popup       = lv_obj_create(main_screen);
    lv_obj_set_size(popup, 350, 211);
    lv_obj_align(popup, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(popup, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置背景颜色

    // 添加标题文本
    lv_obj_t *title_label = lv_label_create(popup);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 0, 10);

    // 添加内容文本
    lv_obj_t *text_label = lv_label_create(popup);
    lv_label_set_text(text_label, text);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, 0);
    // 设置文本标签为自动换行模式
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);

    // 创建"Yes"按钮
    lv_obj_t *yes_btn = lv_btn_create(popup);
    lv_obj_set_size(yes_btn, 120, 40);
    lv_obj_align(yes_btn, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_bg_color(yes_btn, lv_color_hex(0x10A37F), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置按钮颜色
    lv_obj_add_event_cb(yes_btn, yes_btn_click_handler, LV_EVENT_ALL, NULL);

    // 添加"Yes"按钮的标签
    lv_obj_t *yes_label = lv_label_create(yes_btn);
    lv_obj_align(yes_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(yes_label, "YES");

    // 创建"No"按钮
    lv_obj_t *no_btn = lv_btn_create(popup);
    lv_obj_set_size(no_btn, 120, 40);
    lv_obj_align(no_btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_set_style_bg_color(no_btn, lv_color_hex(0x323434), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置按钮颜色
    lv_obj_add_event_cb(no_btn, no_btn_click_handler, LV_EVENT_ALL, NULL);

    // 添加"No"按钮的标签
    lv_obj_t *no_label = lv_label_create(no_btn);
    lv_obj_align(no_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(no_label, "NO");

    return popup;
}

static void yes_btn_click_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        // 在点击"Yes"按钮时进行回调任务的检测和处理
        ESP_LOGI(TAG, "YES button clicked");
        bool bind_flag = true;
        esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &bind_flag, sizeof(bool), portMAX_DELAY);
        // 关闭弹窗
        lv_obj_del(lv_obj_get_parent(target));
    }
}

static void no_btn_click_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        // 点击"No"按钮时不执行任何事件处理，直接关闭弹窗
        ESP_LOGI(TAG, "NO button clicked");
        bool bind_flag = false;

        esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &bind_flag, sizeof(bool), portMAX_DELAY);
        // 关闭弹窗
        lv_obj_del(lv_obj_get_parent(target));
    }
}
