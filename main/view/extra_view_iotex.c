// /**
//  * @file iotex_view.c
//  * @date  21 September 2023

//  * @author Spencer Yan
//  *
//  * @note Description of the file
//  *
//  * @copyright © 2023, Seeed Studio
//  */
#include "extra_view_iotex.h"
#include "indicator_cmd.h"

#include "model_data.h"
#include "view_data.h"

static const char *TAG        = "IOTEX_VIEW";

static bool __g_bind_flag     = false;
static int  __g_scenario_flag = 4;
static int  previous_status   = IOTEX_STATUS_NO_RESPONSE;
/**
 * @brief 当 Confirm 按钮被按下的行为
 * @param e
 */
void fn_bind_confirm(lv_event_t *e)
{
    // lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    ESP_LOGI(TAG, "fn_bind_confirm");
    __g_scenario_flag = 4;
    // pop_up_custom("Confirm", "Please confirm the registrationhas been completed onthe portal");
    // 直接就是绑定, 而不用弹窗
    __g_bind_flag     = true;
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, // 当在 Portal 界面中绑定设备 Confirm 后
                      VIEW_EVENT_IOTEX_USER_CONFIRM,
                      &__g_bind_flag, sizeof(bool), portMAX_DELAY); // To IoTex App
    lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_label_bind, "Registering");
}

/********************** Pop Up **************************/
static void yes_btn_click_handler(lv_event_t *e);
static void no_btn_click_handler(lv_event_t *e);
static void comfirm_btn_click_handler(lv_event_t *e);

// lv_obj_t *pop_up_custom(char *title, char *text)
// {
//     // 创建一个主屏幕
//     lv_obj_t *main_screen = lv_scr_act();

//     // 创建一个弹窗背景
//     lv_obj_t *popup       = lv_obj_create(main_screen);
//     lv_obj_set_size(popup, 350, 211);
//     lv_obj_align(popup, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_bg_color(popup, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置背景颜色

//     // 添加标题文本
//     lv_obj_t *title_label = lv_label_create(popup);
//     lv_label_set_text(title_label, title);
//     lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 0, 10);

//     // 添加内容文本
//     lv_obj_t *text_label = lv_label_create(popup);
//     lv_label_set_text(text_label, text);
//     lv_obj_align(text_label, LV_ALIGN_CENTER, 0, 0);
//     // 设置文本标签为自动换行模式
//     lv_label_set_long_mode(text_label, LV_LABEL_LONG_WRAP);

//     switch (__g_scenario_flag) {
//         case IOTEX_STATUS_DEVICE_SHOULD_ENROLL:
//         case IOTEX_STATUS_DEVICE_SUCCESS:
//             // 创建"Confirm"按钮 在中下方
//             lv_obj_t *confirm_btn = lv_btn_create(popup);
//             lv_obj_set_size(confirm_btn, 120, 40);
//             lv_obj_align(confirm_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
//             lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x10A37F), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置按钮颜色
//             lv_obj_add_event_cb(confirm_btn, comfirm_btn_click_handler, LV_EVENT_ALL, NULL);
//             lv_obj_t *confirm_label = lv_label_create(confirm_btn);
//             lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);
//             // if (__g_scenario_flag == IOTEX_STATUS_DEVICE_CONFIRM_NEEDED)
//             lv_label_set_text(confirm_label, "OK");
//             // else if (__g_scenario_flag == IOTEX_STATUS_DEVICE_SUCCESS)
//             // lv_label_set_text(confirm_label, "OK");
//             break;
//         default:
//             // 创建"Yes"按钮
//             lv_obj_t *yes_btn = lv_btn_create(popup);
//             lv_obj_set_size(yes_btn, 120, 40);
//             lv_obj_align(yes_btn, LV_ALIGN_BOTTOM_LEFT, 20, -20);
//             lv_obj_set_style_bg_color(yes_btn, lv_color_hex(0x10A37F), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置按钮颜色
//             lv_obj_add_event_cb(yes_btn, yes_btn_click_handler, LV_EVENT_ALL, NULL);

//             // 添加"Yes"按钮的标签
//             lv_obj_t *yes_label = lv_label_create(yes_btn);
//             lv_obj_align(yes_label, LV_ALIGN_CENTER, 0, 0);
//             lv_label_set_text(yes_label, "YES");

//             // 创建"No"按钮
//             lv_obj_t *no_btn = lv_btn_create(popup);
//             lv_obj_set_size(no_btn, 120, 40);
//             lv_obj_align(no_btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
//             lv_obj_set_style_bg_color(no_btn, lv_color_hex(0x323434), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置按钮颜色
//             lv_obj_add_event_cb(no_btn, no_btn_click_handler, LV_EVENT_ALL, NULL);

//             // 添加"No"按钮的标签
//             lv_obj_t *no_label = lv_label_create(no_btn);
//             lv_obj_align(no_label, LV_ALIGN_CENTER, 0, 0);
//             lv_label_set_text(no_label, "NO");
//             break;
//     }

//     return popup;
// }
/**
 * @brief 回调函数，在 Button Event 中被处理
 *
 * @param e
 */
static void yes_btn_click_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_obj_t *father_popup   = lv_obj_get_parent(target);
        lv_obj_t *current_screen = lv_obj_get_parent(father_popup);

        ESP_LOGI(TAG, "Pressed YES button");
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, BUTTON_EVENT_YES, NULL, 0, portMAX_DELAY);
        // 关闭弹窗
        lv_obj_del(lv_obj_get_parent(target)); // 銷毀popup
    }
}
static void no_btn_click_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_obj_t *father_popup   = lv_obj_get_parent(target);
        lv_obj_t *current_screen = lv_obj_get_parent(father_popup);

        ESP_LOGI(TAG, "Pressed NO button");
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, BUTTON_EVENT_NO, NULL, 0, portMAX_DELAY);
        // do nothing 关闭弹窗
        lv_obj_del(lv_obj_get_parent(target));
    }
}
static void comfirm_btn_click_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        lv_obj_t *father_popup   = lv_obj_get_parent(target);
        lv_obj_t *current_screen = lv_obj_get_parent(father_popup);
        ESP_LOGI(TAG, "Pressed CONFIRM button");
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, BUTTON_EVENT_CONFIRM, NULL, 0, portMAX_DELAY);
        // do nothing 关闭弹窗
        lv_obj_del(lv_obj_get_parent(target));
    }
}
/********************** Pop Up END **************************/
bool first_time = false;
/* 上电就会进入这里检查 Confirm 按钮是否可被按下 */
static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case VIEW_EVENT_MQTT_IOTEX_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_CFG");
            w3b_cfg_interface *p_cfg = (w3b_cfg_interface *)event_data;
            lv_textarea_set_text(ui_TextArea_SN, p_cfg->sn);
            // lv_textarea_set_text(ui_TextArea_WAD, p_cfg->wallet);
            break;
        }
        case VIEW_EVENT_MQTT_IOTEX_BINDING: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_BINDING");
            bool *p_flag = (bool *)event_data;
            if (*p_flag == true) {
                ESP_LOGI(TAG, "Binding Flag is true");
                __g_bind_flag = true;
                lv_obj_add_state(ui_btn_bind, LV_STATE_DISABLED);
                if (first_time == false) {
                    first_time == true;
                    lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(ui_label_bind, "Registering");
                }
            } else {
                ESP_LOGI(TAG, "Binding Flag is false");
                __g_bind_flag = false;
                lv_obj_clear_state(ui_btn_bind, LV_STATE_DISABLED);
            }
            break;
        }
        case VIEW_DEVICE_ETH_ADDRESS: {
            ESP_LOGI(TAG, "event: VIEW_DEVICE_ETH");
            eth_cfg *view_cfg = (eth_cfg *)event_data;
            lv_textarea_set_text(ui_TextArea_WAD, view_cfg->eth); // ETH_ADDREES

            // lv_textarea_set_text(ui_TextArea_ETH, view_cfg->eth);  // TODO
            break;
        }
        default:
            break;
    }
    lv_port_sem_give();
}

static void custom_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    // LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
    lv_obj_t * mbox = lv_obj_get_parent(btn);
    lv_msgbox_close(mbox);

}

void msgbox(const char *title, const char *message)
{
    static const char *btns[] = {"OK", NULL};

    lv_obj_t *mbox            = lv_msgbox_create(NULL, title, message, btns, false);
    lv_obj_add_event_cb(mbox, custom_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_obj_t *btnm = lv_msgbox_get_btnmatrix(mbox);
    // lv_btnmatrix_set_align(btnm, LV_LABEL_ALIGN_LEFT);

    lv_obj_center(mbox);
}


static void __page_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case IOTEX_STATUS_NO_RESPONSE: { // 没有连上网的时候 或 服务器没有应答
            ESP_LOGI(TAG, "event: IOTEX_STATUS_NO_RESPONSE");
            // do nothing
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_label_get_text(ui_label_bind);
            lv_label_set_text(ui_label_bind, "Confirm");
            // previous_status = IOTEX_STATUS_NO_RESPONSE;
            break;
        }
        case IOTEX_STATUS_DEVICE_SHOULD_ENROLL: { // 设备没有在 Potal 页面上注册，需要注册，弹窗提示
            // 出现这种情况是在 bind_flag = 1时

            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_SHOULD_ENROLL");
            __g_scenario_flag = IOTEX_STATUS_DEVICE_SHOULD_ENROLL;
            /*Function Pop Up*/ // 检测是否在 Portal 上进行注册了
            // pop_up_custom("Required", "Confirm the registration on the portal");
            if (__g_bind_flag == false) {
                __g_bind_flag = true;
                esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);
            }
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(ui_label_bind, "Should Enroll on Portal");
            previous_status = IOTEX_STATUS_DEVICE_SHOULD_ENROLL;
            break;
        }
        case IOTEX_STATUS_DEVICE_CONFIRM_NEEDED: { // 设备（已存在的SN）已经在 Potal 页面上注册，但没有确认
            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_CONFIRM_NEEDED");
            __g_scenario_flag = IOTEX_STATUS_DEVICE_CONFIRM_NEEDED;
            /*Function Pop Up*/ // 弹窗提示是否需要进行确认：检测到当前设备已在 Portal 上进行注册，是否需要进行本地确认信息？

            __g_bind_flag     = false;
            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);

            // pop_up_custom("Required", "Confirm the registration on the Device?");
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(ui_label_bind, "Confirm");
            previous_status = IOTEX_STATUS_DEVICE_CONFIRM_NEEDED;
            break;
        }
        case IOTEX_STATUS_DEVICE_SUCCESS: { // 设备已经绑定成功
            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_SUCCESS");
            __g_scenario_flag = IOTEX_STATUS_DEVICE_SUCCESS;

            // if (__g_bind_flag == true) {
            //     ESP_LOGI(TAG, "IOTEX_STATUS_DEVICE_SUCCESS and __g_bind_flag == true");
            //     lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            //     lv_label_set_text(ui_label_bind, "Registered Successfully");
            //     lv_port_sem_give();
            //     return; // nothing to do, as already registered device.
            // }
            __g_bind_flag     = true;
            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(ui_label_bind, "Registered");
            if (previous_status == IOTEX_STATUS_DEVICE_SHOULD_ENROLL || previous_status == IOTEX_STATUS_DEVICE_CONFIRM_NEEDED) {
                // pop_up_custom("Success", "The device has been successfully registered");
                // static const char *btns[] = {"OK"};
                msgbox("Success", "The device has been successfully registered");
                // msgbox("Success", "The device has been successfully registered");
                // msgbox("Success", "The device has been successfully registered", false);
            }
            previous_status = IOTEX_STATUS_DEVICE_SUCCESS;
            // pop_up_custom("Success", "The device has been successfully registered");
            // 如果当前在 注册页面，就直接跳转到传感数据页面
            // _ui_screen_change(&ui_screen_sensor, LV_SCR_LOAD_ANIM_FADE_IN, 200, 300, &ui_screen_sensor_screen_init);

            break;
        }
        default:
            break;
    }
    lv_port_sem_give();
}


static void __button_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    ESP_LOGI(TAG, "__g_scenario_flag == %d", __g_scenario_flag); // 0,1,2,3 ;4
    switch (id) {
        case BUTTON_EVENT_YES: { // 点击Confirm 按钮和 弹出 IOTEX_STATUS_DEVICE_CONFIRM_NEEDED 时窗时，点击"Yes"按钮时的行为
            ESP_LOGI(TAG, "event: BUTTON_EVENT_YES");
            if (__g_scenario_flag == IOTEX_STATUS_DEVICE_CONFIRM_NEEDED) { // 当服务器需要进行本地设备 Confirm 时
                if (lv_scr_act() == ui_screen_binding)
                    break;
                else {
                    _ui_screen_change(&ui_screen_binding, LV_SCR_LOAD_ANIM_FADE_IN, 200, 100, &ui_screen_binding_screen_init);
                }
            }
            // else if (__g_scenario_flag == 4) // 设备绑定页面，点击"yes"按钮时，修改bind_flag，需要传递
            // {
            //     bool bind_flag = false;
            //     // 在点击"Yes"按钮时进行回调任务的检测和处理

            //     ESP_LOGI(TAG, "Bind_flag Confirmed");

            //     bind_flag = true;
            //     esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, // 当在 Portal 界面中绑定设备 Confirm 后
            //                       VIEW_EVENT_IOTEX_USER_CONFIRM,
            //                       &bind_flag, sizeof(bool), portMAX_DELAY); // To IoTex App
            // }

            break;
        }
        case BUTTON_EVENT_NO: {
            ESP_LOGI(TAG, "event: BUTTON_EVENT_NO");
            // do nothing;

            break;
        }
        case BUTTON_EVENT_CONFIRM: {

            if (__g_scenario_flag == IOTEX_STATUS_DEVICE_SHOULD_ENROLL) {
                // do noting;
            } else if (__g_scenario_flag == IOTEX_STATUS_DEVICE_SUCCESS) {
                // do noting;
            } else if (__g_scenario_flag == IOTEX_STATUS_DEVICE_CONFIRM_NEEDED) {

            } else if (__g_scenario_flag == 4) // 设备绑定页面，点击"Confirm"按钮时，修改bind_flag，需要传递
            {
                bool bind_flag = false;
                // 在点击"Confirm"按钮时进行回调任务的检测和处理

                ESP_LOGI(TAG, "Bind_flag Confirmed");

                bind_flag = true;
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, // 当在 Portal 界面中绑定设备 Confirm 后
                                  VIEW_EVENT_IOTEX_USER_CONFIRM,
                                  &bind_flag, sizeof(bool), portMAX_DELAY); // To IoTex App
            }
        }
        default:
            break;
    }
    __g_scenario_flag = 4; // send to VIEW_EVENT_IOTEX_USER_CONFIRM
    lv_port_sem_give();
}


int iotex_view_cfg_event_register(void)
{

    /* 触发而显示到界面上 */
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                                                             __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_BINDING,
                                                             __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_DEVICE_ETH_ADDRESS,
                                                             __view_event_handler, NULL, NULL));
    /* END */
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, IOTEX_STATUS_NO_RESPONSE,
                                                             __page_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SHOULD_ENROLL,
                                                             __page_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_CONFIRM_NEEDED,
                                                             __page_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SUCCESS,
                                                             __page_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, BUTTON_EVENT_YES,
                                                             __button_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, BUTTON_EVENT_NO,
                                                             __button_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, BUTTON_EVENT_CONFIRM,
                                                             __button_event_handler, NULL, NULL));
}