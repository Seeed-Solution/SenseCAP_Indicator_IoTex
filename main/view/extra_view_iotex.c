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

static void custom_event_cb(lv_event_t *e)
{
    lv_obj_t *btn  = lv_event_get_target(e);
    // LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
    lv_obj_t *mbox = lv_obj_get_parent(btn);
    lv_msgbox_close(mbox);
}
void msgbox(const char *title, const char *message)
{
  static const char *btns_point[] = {"OK", NULL};

  lv_obj_t *mbox = lv_msgbox_create(NULL, title, message, btns_point, false);
  lv_obj_add_event_cb(mbox, custom_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // lv_obj_set_size(mbox, LV_DPI_DEF * 2.5, LV_SIZE_CONTENT);
  /************ 标题 ***************/
  lv_obj_t *msg_title = lv_msgbox_get_title(mbox);
  lv_obj_set_style_text_font(msg_title, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_color(msg_title, lv_color_hex(0x007FFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(msg_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_letter_space(msg_title, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_line_space(msg_title, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_decor(msg_title, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(msg_title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_width(msg_title, 220);
  // lv_obj_center(msg_title);

  /*********** 内容 **************/
  lv_obj_t *contents_label = lv_msgbox_get_text(mbox);
  lv_obj_set_style_text_font(contents_label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_color(contents_label, lv_color_hex(0x007FFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(contents_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_letter_space(contents_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_line_space(contents_label, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_decor(contents_label, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_text_opa(contents_label, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(contents_label, 220);
  lv_label_set_long_mode(contents_label, LV_LABEL_LONG_WRAP);

  // lv_obj_set_width(contents_label, 220);
  // lv_obj_set_width(contents_label, 220);
  //   lv_label_set_text(contents_label, message);
  //   lv_obj_align(contents_label, LV_ALIGN_CENTER, 0, 0);
  //   lv_obj_center(contents_label);

  // 打印两者内容
  // LV_LOG_USER("title: %s", lv_label_get_text(title_label));
  // LV_LOG_USER("contents: %s", lv_label_get_text(contents_label));

  /*********** Button ***********/
  lv_obj_t *btns = lv_msgbox_get_btns(mbox);
  lv_obj_set_width(btns, 220);
  // lv_obj_set_size(btns, (220), 40);
  // lv_coord_t btns_h = lv_obj_get_height(btns);
  // lv_obj_set_y(btns, lv_obj_get_height(mbox) - btns_h);
  // lv_obj_set_x(btns, (LV_DPI_DEF * 2.5)/2 - 220/2);

  // CENTER display btn
  lv_obj_center(btns);
  lv_obj_center(mbox);
}


// void msgbox(lv_obj_t *parent, const char *title, const char *txt, const char *btn_txts[], bool add_close_btn)
// {
//     ESP_LOGI(TAG, "msgbox");
//     // 创建基本的消息框
//     lv_obj_t *msgbox = lv_msgbox_create(parent, title, txt, btn_txts, add_close_btn);
//     ESP_LOGI(TAG, "msgbox");
//     // 创建并初始化背景样式
//     // static lv_style_t bg_style;
//     // lv_style_init(&bg_style);
//     // lv_style_set_bg_color(&bg_style, lv_color_hex(0xFFFFFF));
//     // ... 其他背景样式设置

//     // 创建并初始化按钮样式
//     static lv_style_t btn_style;
//     lv_style_init(&btn_style);
//     lv_style_set_bg_color(&btn_style, lv_color_hex(0x007FFF));
//     // ... 其他按钮样式设置

//     // 获取消息框的按钮矩阵对象
//     lv_obj_t *btns = lv_msgbox_get_btns(msgbox);

//     // 将样式应用到消息框和按钮矩阵
//     // lv_obj_add_style(msgbox, &bg_style, 0);
//     lv_obj_add_style(btns, &btn_style, 0);

//     // 调整按钮矩阵的大小和位置以居中“OK”按钮
//     lv_obj_set_size(btns, LV_PCT(100), LV_SIZE_CONTENT);
//     lv_obj_align(btns, LV_ALIGN_CENTER, 0, 0);

//     return msgbox;
// }

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

    // 内容是否为空？
    // if (strlen(lv_textarea_get_text(ui_TextArea_SN)) == 0 || strlen(lv_textarea_get_text(ui_TextArea_WAD)) == 0) {
    //     ESP_LOGW(TAG, "SN or Wallet Address is empty, please input");
    //     // 如果为空，是不能进行操作的
    //     // msgbox("Warning", "SN is empty, please input");
    //     msgbox(NULL, "Warning", "SN or Wallet Address is empty, please input", NULL, false);
    //     return;
    // }

    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, // 当在 Portal 界面中绑定设备 Confirm 后
                      VIEW_EVENT_IOTEX_USER_CONFIRM,
                      &__g_bind_flag, sizeof(bool), portMAX_DELAY); // To IoTex App
    lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_label_bind, "Registering");
}

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
            lv_textarea_set_text(ui_TextArea_WAD, p_cfg->wallet);
            break;
        }
        case VIEW_EVENT_MQTT_IOTEX_BINDING: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_BINDING");
            bool *p_flag = (bool *)event_data;
            if (*p_flag == true) {
                ESP_LOGI(TAG, "Binding Flag is true");
                __g_bind_flag = true;
                lv_obj_add_state(ui_btn_bind, LV_STATE_DISABLED);
                if (first_time) {
                    first_time = false;
                    lv_obj_clear_state(ui_btn_bind, LV_STATE_USER_1);
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
        // case VIEW_DEVICE_ETH_ADDRESS: {
        //     ESP_LOGI(TAG, "event: VIEW_DEVICE_ETH");
        //     eth_cfg *view_cfg = (eth_cfg *)event_data;
        //     lv_textarea_set_text(ui_TextArea_WAD, view_cfg->eth); // ETH_ADDREES
        //     break;
        // }
        default:
            break;
    }
    lv_port_sem_give();
}

static void __page_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case IOTEX_STATUS_NO_RESPONSE: { // 没有连上网的时候 或 服务器没有应答
            ESP_LOGI(TAG, "event: IOTEX_STATUS_NO_RESPONSE");
            // do nothing
            // lv_label_get_text(ui_label_bind);
            // lv_obj_clear_state(ui_btn_bind, LV_STATE_USER_1);
            // lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            // lv_label_set_text(ui_label_bind, "Confirm");
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
            lv_obj_clear_state(ui_btn_bind, LV_STATE_USER_1);
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(ui_label_bind, "Should Enroll on Portal");
            msgbox("Required", "Should Enroll on Portal");
            // static const char *btn_txts[] = {"OK", NULL};
            // msgbox(NULL, "Required", "Should Enroll on Portal", btn_txts, false);
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
            lv_obj_clear_state(ui_btn_bind, LV_STATE_USER_1);
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
            lv_obj_set_style_text_font(ui_label_bind, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_label_set_text(ui_label_bind, "Registered");
            lv_obj_add_state(ui_btn_bind, LV_STATE_USER_1); /// States

            esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &__g_bind_flag, sizeof(bool), portMAX_DELAY);
            // if (previous_status == IOTEX_STATUS_DEVICE_SHOULD_ENROLL || previous_status == IOTEX_STATUS_DEVICE_CONFIRM_NEEDED) {
            //     // pop_up_custom("Success", "The device has been successfully registered");
            //     // static const char *btns[] = {"OK"};
            //     msgbox("Success", "The device has been successfully registered");
            //     // msgbox("Success", "The device has been successfully registered");
            //     // msgbox("Success", "The device has been successfully registered", false);
            // }
            previous_status = IOTEX_STATUS_DEVICE_SUCCESS;
            // pop_up_custom("Success", "The device has been successfully registered"); // 不同谈弹串
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
    first_time = true;
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