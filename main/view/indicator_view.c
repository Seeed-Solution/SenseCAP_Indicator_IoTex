#include "indicator_view.h"
#include "freertos/semphr.h"
#include "indicator_util.h"

#include "esp_event.h"
#include "esp_event_base.h"

#include <time.h>

#include "extra_view_display.h"
#include "extra_view_iotex.h"
#include "extra_view_sensor.h"
#include "extra_view_time.h"

static const char *TAG           = "view";

lv_obj_t         *Jetson_tabView = NULL;
SemaphoreHandle_t JetsonTableMutex;
const TickType_t  xTicksToWait = 1000 / portTICK_PERIOD_MS;

ESP_EVENT_DEFINE_BASE(VIEW_EVENT_BASE); // View
esp_event_loop_handle_t view_event_handle;

LV_IMG_DECLARE(ui_img_lock_png);
/*****************************************************************/
// wifi config
/*****************************************************************/

static void wifi_list_init(void);

static lv_obj_t *ui_wifi_scan_wait;

static lv_obj_t *ui_wifi_list = NULL;
static lv_obj_t *ui_wifi_connecting;
static lv_obj_t *password_kb;
static lv_obj_t *ui_password_input;
static lv_obj_t *ui_wifi_connect_ret;

static char __g_cur_wifi_ssid[32];
;

static uint8_t password_ready = false;

static void event_wifi_connect_cancel(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        if (ui_wifi_connecting != NULL) {
            lv_obj_del(ui_wifi_connecting);
            ui_wifi_connecting = NULL;
            lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
        }

        if (password_kb != NULL) {
            lv_obj_del(password_kb);
            password_kb = NULL;
        }
    }
}

static void event_wifi_connect_join(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        struct view_data_wifi_config cfg;
        if (password_kb != NULL) {
            cfg.have_password    = true;
            const char *password = lv_textarea_get_text(ui_password_input);
            strncpy((char *)cfg.password, password, sizeof(cfg.ssid));
            ESP_LOGI(TAG, "lv_obj_del: password_kb");
            lv_obj_del(password_kb);
            password_kb = NULL;
        } else {
            cfg.have_password = false;
        }

        if (ui_wifi_connecting != NULL) {
            strncpy((char *)cfg.ssid, (char *)__g_cur_wifi_ssid, sizeof(cfg.ssid));
            ESP_LOGI(TAG, "ssid: %s", cfg.ssid);

            ESP_LOGI(TAG, "lv_obj_del: ui_wifi_connecting");
            lv_obj_t *o        = ui_wifi_connecting;
            ui_wifi_connecting = NULL;
            lv_obj_del(o);

            ESP_LOGI(TAG, "lv_obj_del: ui_wifi_connecting  end");

            lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
        }
        lv_obj_clear_flag(ui_wifi_scan_wait, LV_OBJ_FLAG_HIDDEN);
        if (ui_wifi_list) {
            lv_obj_add_flag(ui_wifi_list, LV_OBJ_FLAG_HIDDEN);
        }
        // wifi_list_init();
        esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WIFI_CONNECT, &cfg, sizeof(cfg), portMAX_DELAY);
    }
}

static void event_wifi_password_input(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED) {
        if (ui_wifi_connecting != NULL && ta != NULL) {

            lv_obj_t *join_btn   = (lv_obj_t *)e->user_data;

            const char *password = lv_textarea_get_text(ta);
            if (password != NULL) {
                if ((strlen(password) >= 8) && !password_ready) {
                    password_ready = true;
                    lv_obj_add_flag(join_btn, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(lv_obj_get_child(join_btn, 0), lv_color_hex(0x529d53), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                if ((strlen(password) < 8) && password_ready) {
                    password_ready = false;
                    lv_obj_clear_flag(join_btn, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(lv_obj_get_child(join_btn, 0), lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
}

static void event_keyboard_ready(lv_event_t *e)
{
    if (password_ready == true) {
        lv_event_send((lv_obj_t *)e->user_data, LV_EVENT_CLICKED, NULL);
    }
}

static lv_obj_t *create_wifi_connecting(lv_obj_t *parent, const char *p_ssid, bool have_password)
{
    lv_obj_t *wifi_con = lv_obj_create(parent);
    lv_obj_set_width(wifi_con, 420);
    lv_obj_set_height(wifi_con, 420);
    lv_obj_set_x(wifi_con, 0);
    lv_obj_set_y(wifi_con, 0);
    lv_obj_set_align(wifi_con, LV_ALIGN_CENTER);
    lv_obj_clear_flag(wifi_con, LV_OBJ_FLAG_SCROLLABLE); /// Flags


    lv_obj_t *wifi_connect_cancel = lv_btn_create(wifi_con);
    lv_obj_set_width(wifi_connect_cancel, 100);
    lv_obj_set_height(wifi_connect_cancel, 50);
    lv_obj_set_align(wifi_connect_cancel, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(wifi_connect_cancel, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_connect_cancel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(wifi_connect_cancel, event_wifi_connect_cancel, LV_EVENT_CLICKED, NULL);

    lv_obj_t *wifi_connect_cancel_title = lv_label_create(wifi_connect_cancel);
    lv_obj_set_width(wifi_connect_cancel_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_cancel_title, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(wifi_connect_cancel_title, "Cancel");
    lv_obj_set_style_text_font(wifi_connect_cancel_title, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);


    lv_obj_t *wifi_connect_join = lv_btn_create(wifi_con);
    lv_obj_set_width(wifi_connect_join, 70);
    lv_obj_set_height(wifi_connect_join, 50);
    lv_obj_set_align(wifi_connect_join, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_bg_color(wifi_connect_join, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_connect_join, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(wifi_connect_join, event_wifi_connect_join, LV_EVENT_CLICKED, NULL);

    lv_obj_t *wifi_connect_join_title = lv_label_create(wifi_connect_join);
    lv_obj_set_width(wifi_connect_join_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_join_title, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(wifi_connect_join_title, "Join");
    lv_obj_set_style_text_font(wifi_connect_join_title, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (!have_password) {
        lv_obj_set_style_text_color(wifi_connect_join_title, lv_color_hex(0x529d53), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_obj_clear_flag(wifi_connect_join, LV_OBJ_FLAG_CLICKABLE);
    }


    lv_obj_t *wifi_connect_ssid = lv_label_create(wifi_con);
    lv_obj_set_width(wifi_connect_ssid, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_ssid, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(wifi_connect_ssid, LV_ALIGN_TOP_MID);
    lv_obj_set_y(wifi_connect_ssid, 50);

    lv_label_set_text_fmt(wifi_connect_ssid, "%s", p_ssid);
    lv_obj_set_style_text_font(wifi_connect_ssid, &ui_font_font1, LV_PART_MAIN | LV_STATE_DEFAULT);


    if (have_password) {
        lv_obj_t *input_password_title = lv_label_create(wifi_con);
        lv_obj_set_width(input_password_title, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height(input_password_title, LV_SIZE_CONTENT); /// 1
        lv_obj_set_align(input_password_title, LV_ALIGN_TOP_MID);
        lv_obj_set_y(input_password_title, 100);
        lv_obj_set_x(input_password_title, -80);

        lv_label_set_text_fmt(input_password_title, "Input password");
        // lv_obj_set_style_text_font(input_password_title, &ui_font_font0, LV_PART_MAIN| LV_STATE_DEFAULT);


        password_ready    = false;

        ui_password_input = lv_textarea_create(wifi_con);
        lv_textarea_set_text(ui_password_input, "");
        // lv_textarea_set_password_mode(ui_password_input, true);  todo
        lv_textarea_set_one_line(ui_password_input, true);
        lv_obj_set_width(ui_password_input, lv_pct(80));
        lv_obj_set_align(ui_password_input, LV_ALIGN_TOP_MID);
        lv_obj_set_y(ui_password_input, 130);
        // lv_obj_set_x( ui_password_input, -80);
        lv_obj_set_style_bg_color(ui_password_input, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_password_input, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(ui_password_input, event_wifi_password_input, LV_EVENT_VALUE_CHANGED, (void *)wifi_connect_join);

        password_kb = lv_keyboard_create(parent);
        lv_keyboard_set_textarea(password_kb, ui_password_input);
        lv_keyboard_set_popovers(password_kb, true);
        lv_obj_add_event_cb(password_kb, event_keyboard_ready, LV_EVENT_READY, wifi_connect_join);
    }

    return wifi_con;
}

static void event_wifi_connect(lv_event_t *e)
{
    bool  have_password  = false;
    char *p_wifi_ssid    = "";

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        if (lv_indev_get_type(lv_indev_get_act()) == LV_INDEV_TYPE_POINTER) {

            // todo connect wifi
            if (ui_wifi_connecting == NULL) {
                p_wifi_ssid = lv_list_get_btn_text(ui_wifi_list, ta);
                if (lv_obj_get_child_cnt(ta) > 2) {
                    have_password = true;
                }
                strncpy((char *)__g_cur_wifi_ssid, p_wifi_ssid, sizeof(__g_cur_wifi_ssid));
                lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
                ui_wifi_connecting = create_wifi_connecting(lv_layer_top(), (const char *)p_wifi_ssid, have_password);
                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
                lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
            }
        }
    }
}


void timer_wifi_connect_ret_close(lv_timer_t *timer)
{
    if (ui_wifi_connect_ret != NULL) {
        lv_obj_del(ui_wifi_connect_ret);
        ui_wifi_connect_ret = NULL;
        lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
    }
}

static void ui_wifi_connect_ret_init(struct view_data_wifi_connet_ret_msg *p_msg)
{
    ui_wifi_connect_ret = lv_obj_create(lv_layer_top());
    lv_obj_set_width(ui_wifi_connect_ret, 300);
    lv_obj_set_height(ui_wifi_connect_ret, 150);
    lv_obj_set_x(ui_wifi_connect_ret, 0);
    lv_obj_set_y(ui_wifi_connect_ret, 0);
    lv_obj_set_align(ui_wifi_connect_ret, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_wifi_connect_ret, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_wifi_connect_ret, lv_palette_main(LV_PALETTE_GREY), 0);

    lv_obj_t *wifi_connect_ret_msg = lv_label_create(ui_wifi_connect_ret);
    lv_obj_set_width(wifi_connect_ret_msg, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_ret_msg, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(wifi_connect_ret_msg, LV_ALIGN_CENTER);

    lv_label_set_text_fmt(wifi_connect_ret_msg, "%s", p_msg->msg);
    lv_obj_set_style_text_font(wifi_connect_ret_msg, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_timer_t *timer = lv_timer_create(timer_wifi_connect_ret_close, 1500, NULL);
    lv_timer_set_repeat_count(timer, 1);
}

static void event_wifi_delete(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {

        esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WIFI_CFG_DELETE, NULL, 0, portMAX_DELAY);

        if (ui_wifi_connecting != NULL) {
            lv_obj_del(ui_wifi_connecting);
            ui_wifi_connecting = NULL;
            lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
        }

        lv_obj_clear_flag(ui_wifi_scan_wait, LV_OBJ_FLAG_HIDDEN);
        if (ui_wifi_list) {
            lv_obj_add_flag(ui_wifi_list, LV_OBJ_FLAG_HIDDEN);
        }
        esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WIFI_LIST_REQ, NULL, 0, portMAX_DELAY); // updata wifi list
    }
}

static lv_obj_t *ui_wifi_details(lv_obj_t *parent, const char *p_ssid)
{
    lv_obj_t *wifi_con = lv_obj_create(parent);
    lv_obj_set_width(wifi_con, 300);
    lv_obj_set_height(wifi_con, 200);
    lv_obj_set_x(wifi_con, 0);
    lv_obj_set_y(wifi_con, 0);
    lv_obj_set_align(wifi_con, LV_ALIGN_CENTER);
    lv_obj_clear_flag(wifi_con, LV_OBJ_FLAG_SCROLLABLE); /// Flags


    lv_obj_t *wifi_connect_cancel = lv_btn_create(wifi_con);
    lv_obj_set_width(wifi_connect_cancel, 100);
    lv_obj_set_height(wifi_connect_cancel, 50);
    lv_obj_set_align(wifi_connect_cancel, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_style_bg_color(wifi_connect_cancel, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_connect_cancel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(wifi_connect_cancel, event_wifi_connect_cancel, LV_EVENT_CLICKED, NULL);

    lv_obj_t *wifi_connect_cancel_title = lv_label_create(wifi_connect_cancel);
    lv_obj_set_width(wifi_connect_cancel_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_cancel_title, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(wifi_connect_cancel_title, "Cancel");
    lv_obj_set_style_text_font(wifi_connect_cancel_title, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(wifi_connect_cancel_title, lv_color_hex(0x529d53), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *wifi_connect_delete = lv_btn_create(wifi_con);
    lv_obj_set_width(wifi_connect_delete, 100);
    lv_obj_set_height(wifi_connect_delete, 50);
    lv_obj_set_align(wifi_connect_delete, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_style_bg_color(wifi_connect_delete, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_connect_delete, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(wifi_connect_delete, event_wifi_delete, LV_EVENT_CLICKED, NULL); // todo

    lv_obj_t *wifi_connect_delete_title = lv_label_create(wifi_connect_delete);
    lv_obj_set_width(wifi_connect_delete_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_delete_title, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(wifi_connect_delete_title, "Delete");
    lv_obj_set_style_text_font(wifi_connect_delete_title, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(wifi_connect_delete_title, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *wifi_connect_ssid = lv_label_create(wifi_con);
    lv_obj_set_width(wifi_connect_ssid, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(wifi_connect_ssid, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(wifi_connect_ssid, LV_ALIGN_CENTER);
    lv_obj_set_y(wifi_connect_ssid, -20);

    lv_label_set_text_fmt(wifi_connect_ssid, "%s", p_ssid);
    lv_obj_set_style_text_font(wifi_connect_ssid, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);

    return wifi_con;
}


static void event_wifi_details(lv_event_t *e)
{
    char *p_wifi_ssid    = "";

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t       *ta   = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED) {
        if (lv_indev_get_type(lv_indev_get_act()) == LV_INDEV_TYPE_POINTER) {


            if (ui_wifi_connecting == NULL) {
                p_wifi_ssid = lv_list_get_btn_text(ui_wifi_list, ta);

                strncpy((char *)__g_cur_wifi_ssid, p_wifi_ssid, sizeof(__g_cur_wifi_ssid));
                lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
                ui_wifi_connecting = ui_wifi_details(lv_layer_top(), (const char *)p_wifi_ssid);

                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
                lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
            }
        }
    }
}

static void create_wifi_item(lv_obj_t *parent, const char *p_ssid, bool have_password, int rssi, bool is_connect)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_width(btn, 380);
    lv_obj_set_height(btn, 50);
    lv_obj_set_align(btn, LV_ALIGN_CENTER);
    if (is_connect) {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x529d53), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn, event_wifi_details, LV_EVENT_CLICKED, NULL);
    } else {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x2c2c2c), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(btn, event_wifi_connect, LV_EVENT_CLICKED, NULL);
    }

    lv_obj_t *wifi_ssid = lv_label_create(btn);
    lv_label_set_text_fmt(wifi_ssid, "%s", p_ssid);
    lv_obj_set_width(wifi_ssid, LV_SIZE_CONTENT);
    lv_obj_set_height(wifi_ssid, LV_SIZE_CONTENT);
    lv_obj_set_align(wifi_ssid, LV_ALIGN_LEFT_MID);
    lv_obj_set_style_text_font(wifi_ssid, &ui_font_font0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_x(wifi_ssid, 10);

    lv_obj_t *wifi_rssi_icon = lv_img_create(btn);
    lv_obj_set_width(wifi_rssi_icon, LV_SIZE_CONTENT); /// 22
    lv_obj_set_height(wifi_rssi_icon, LV_SIZE_CONTENT);
    lv_obj_set_align(wifi_rssi_icon, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(wifi_rssi_icon, -10);


    switch (wifi_rssi_level_get(rssi)) {
        case 1:
            lv_img_set_src(wifi_rssi_icon, &ui_img_wifi_1_png);
            break;
        case 2:
            lv_img_set_src(wifi_rssi_icon, &ui_img_wifi_2_png);
            break;
        case 3:
            lv_img_set_src(wifi_rssi_icon, &ui_img_wifi_3_png);
            break;
        default:
            break;
    }

    if (have_password) {
        lv_obj_t *wifi_lock_icon = lv_img_create(btn);
        lv_img_set_src(wifi_lock_icon, &ui_img_lock_png);
        lv_obj_set_width(wifi_lock_icon, LV_SIZE_CONTENT); /// 22
        lv_obj_set_height(wifi_lock_icon, LV_SIZE_CONTENT);
        lv_obj_set_align(wifi_lock_icon, LV_ALIGN_RIGHT_MID);
        lv_obj_set_x(wifi_lock_icon, -60);
    }
}

static void wifi_list_init(void)
{
    if (ui_wifi_list != NULL) {
        ESP_LOGI(TAG, "lv_obj_del: ui_wifi_list");
        // lv_obj_clean(ui_wifi_list);
        lv_obj_del(ui_wifi_list);
        ui_wifi_list = NULL;
    }
    ui_wifi_list = lv_list_create(ui_screen_wifi);
    lv_obj_set_style_pad_row(ui_wifi_list, 8, 0);

    lv_obj_set_align(ui_wifi_list, LV_ALIGN_CENTER);
    lv_obj_set_width(ui_wifi_list, 420);
    lv_obj_set_height(ui_wifi_list, 330);
    lv_obj_set_x(ui_wifi_list, 0);
    lv_obj_set_y(ui_wifi_list, 35);

    lv_obj_set_style_bg_color(ui_wifi_list, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_wifi_list, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_wifi_list, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(ui_wifi_list, LV_OBJ_FLAG_HIDDEN);
}


void __ui_event_wifi_config(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t       *target     = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_clear_flag(ui_wifi_scan_wait, LV_OBJ_FLAG_HIDDEN);
        if (ui_wifi_list) { // 为了防止wifi_list_init()重复创建
            lv_obj_add_flag(ui_wifi_list, LV_OBJ_FLAG_HIDDEN);
        }
        // wifi_list_init();
        esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WIFI_LIST_REQ, NULL, 0, portMAX_DELAY);
    }
}


void wifi_list_event_init(void)
{
    ui_wifi_scan_wait = lv_spinner_create(ui_screen_wifi, 1000, 60);
    lv_obj_set_size(ui_wifi_scan_wait, 50, 50);
    lv_obj_center(ui_wifi_scan_wait);
    lv_obj_add_event_cb(ui_screen_wifi, __ui_event_wifi_config, LV_EVENT_SCREEN_LOAD_START, NULL);
}

/***********************************************************************************************************/

static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case VIEW_EVENT_SCREEN_START: {
            uint8_t screen = *(uint8_t *)event_data;
            if (screen == SCREEN_WIFI_CONFIG) {
                lv_disp_load_scr(ui_screen_wifi);
            }
        }
        case VIEW_EVENT_TIME: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME");
            bool time_format_24 = true;
            if (event_data) {
                time_format_24 = *(bool *)event_data;
            }

            time_t    now      = 0;
            struct tm timeinfo = {0};
            char     *p_wday_str;

            time(&now);
            localtime_r(&now, &timeinfo);
            char buf_h[3];
            char buf_m[3];
            char buf[6];
            int  hour = timeinfo.tm_hour;

            if (!time_format_24) {
                if (hour >= 13 && hour <= 23) {
                    hour = hour - 12;
                }
            }
            lv_snprintf(buf_h, sizeof(buf_h), "%02d", hour);
            lv_label_set_text(ui_hour_dis, buf_h);
            lv_snprintf(buf_m, sizeof(buf_m), "%02d", timeinfo.tm_min);
            lv_label_set_text(ui_min_dis, buf_m);

            lv_snprintf(buf, sizeof(buf), "%02d:%02d", hour, timeinfo.tm_min);
            lv_label_set_text(ui_time2, buf);
            lv_label_set_text(ui_time3, buf);

            switch (timeinfo.tm_wday) {
                case 0: p_wday_str = "Sunday"; break;
                case 1: p_wday_str = "Monday"; break;
                case 2: p_wday_str = "Tuesday"; break;
                case 3: p_wday_str = "Wednesday"; break;
                case 4: p_wday_str = "Thursday"; break;
                case 5: p_wday_str = "Friday"; break;
                case 6: p_wday_str = "Sunday"; break;
                default: p_wday_str = ""; break;
            }
            char buf1[32];
            lv_snprintf(buf1, sizeof(buf1), "%s, %02d / %02d / %04d", p_wday_str, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
            lv_label_set_text(ui_date, buf1);
            break;
        }

            // case VIEW_EVENT_TIME_CFG_UPDATE: {
            //     ESP_LOGI(TAG, "event: VIEW_EVENT_TIME_CFG_UPDATE");
            //     struct view_data_time_cfg *p_cfg = (struct view_data_time_cfg *)event_data;

            //     if (p_cfg->time_format_24) {
            //         lv_dropdown_set_selected(ui_time_format_cfg, 0);
            //     } else {
            //         lv_dropdown_set_selected(ui_time_format_cfg, 1);
            //     }

            //     if (p_cfg->auto_update) {
            //         lv_obj_add_state(ui_auto_update_cfg, LV_STATE_CHECKED);
            //         lv_obj_add_flag(ui_date_time, LV_OBJ_FLAG_HIDDEN);
            //     } else {
            //         lv_obj_clear_state(ui_auto_update_cfg, LV_STATE_CHECKED);
            //         lv_obj_clear_flag(ui_date_time, LV_OBJ_FLAG_HIDDEN);
            //     }


            //     struct tm *p_tm = localtime(&p_cfg->time);
            //     char       buf[32];
            //     if (p_tm->tm_year + 1900 > 1970) {
            //         lv_snprintf(buf, sizeof(buf), "%02d/%02d/%d", p_tm->tm_mday, p_tm->tm_mon + 1, p_tm->tm_year + 1900);
            //         lv_textarea_set_text(ui_date_cfg, buf);
            //     }

            //     lv_roller_set_selected(ui_hour_cfg, p_tm->tm_hour, LV_ANIM_OFF);
            //     lv_roller_set_selected(ui_min_cfg, p_tm->tm_min, LV_ANIM_OFF);
            //     lv_roller_set_selected(ui_sec_cfg, p_tm->tm_sec, LV_ANIM_OFF);

            //     if (p_cfg->auto_update_zone) {
            //         lv_obj_add_state(ui_zone_auto_update_cfg, LV_STATE_CHECKED);
            //         lv_obj_add_flag(ui_time_zone, LV_OBJ_FLAG_HIDDEN);
            //     } else {
            //         lv_obj_clear_state(ui_zone_auto_update_cfg, LV_STATE_CHECKED);
            //         lv_obj_clear_flag(ui_time_zone, LV_OBJ_FLAG_HIDDEN);
            //     }

            //     if (p_cfg->zone >= 0) {
            //         lv_dropdown_set_selected(ui_time_zone_sign_cfg_, 0);
            //         lv_dropdown_set_selected(ui_time_zone_num_cfg, p_cfg->zone);
            //     } else {
            //         lv_dropdown_set_selected(ui_time_zone_sign_cfg_, 1);
            //         lv_dropdown_set_selected(ui_time_zone_num_cfg, 0 - p_cfg->zone);
            //     }

            //     if (p_cfg->daylight) {
            //         lv_obj_add_state(ui_daylight_cfg, LV_STATE_CHECKED);
            //     } else {
            //         lv_obj_clear_state(ui_daylight_cfg, LV_STATE_CHECKED);
            //     }
            //     break;
            // }

            // case VIEW_EVENT_DISPLAY_CFG: {
            //     ESP_LOGI(TAG, "event: VIEW_EVENT_DISPLAY_CFG");
            //     struct view_data_display *p_cfg = (struct view_data_display *)event_data;

            //     lv_slider_set_value(ui_brighness_cfg, p_cfg->brightness, LV_ANIM_OFF);
            //     if (p_cfg->sleep_mode_en) {
            //         lv_obj_clear_state(ui_screen_always_on_cfg, LV_STATE_CHECKED);
            //         lv_obj_clear_flag(ui_turn_off_after_time, LV_OBJ_FLAG_HIDDEN);
            //         char buf[32];
            //         lv_snprintf(buf, sizeof(buf), "%d", p_cfg->sleep_mode_time_min);
            //         lv_textarea_set_text(ui_turn_off_after_time_cfg, buf);
            //         // lv_label_set_text_fmt(ui_turn_off_after_time_cfg, "%"LV_PRIu32, p_cfg->sleep_mode_time_min);
            //     } else {
            //         lv_obj_add_state(ui_screen_always_on_cfg, LV_STATE_CHECKED);
            //         lv_obj_add_flag(ui_turn_off_after_time, LV_OBJ_FLAG_HIDDEN);
            //     }

            //     break;
            // }

        case VIEW_EVENT_CITY: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_CITY");
            char *p_data = (char *)event_data;
            lv_label_set_text(ui_city, p_data);
            break;
        }


        case VIEW_EVENT_WIFI_ST: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            struct view_data_wifi_st *p_st = (struct view_data_wifi_st *)event_data;

            uint8_t *p_src                 = NULL;

            // todo is_network
            if (p_st->is_connected) {
                switch (wifi_rssi_level_get(p_st->rssi)) {
                    case 1:
                        p_src = &ui_img_wifi_1_png;
                        break;
                    case 2:
                        p_src = &ui_img_wifi_2_png;
                        break;
                    case 3:
                        p_src = &ui_img_wifi_3_png;
                        break;
                    default:
                        break;
                }

            } else {
                p_src = &ui_img_wifi_disconet_png;
            }

            lv_img_set_src(ui_wifi_st_1, (void *)p_src); // screen_time_screen
            lv_img_set_src(ui_wifi_st_2, (void *)p_src); // screen_sensor
            lv_img_set_src(ui_wifi_st_3, (void *)p_src); // screen_setting
            // lv_img_set_src(ui_wifi_st_4, (void *)p_src); // screen_display control light
            // lv_img_set_src(ui_wifi_st_5, (void *)p_src); //screen_date_time
            lv_img_set_src(ui_wifi_st_6, (void *)p_src); // screen_wifi
            lv_img_set_src(ui_wifi_st_7, (void *)p_src); // screen_binding
            break;
        }
        case VIEW_EVENT_WIFI_LIST: {
            ESP_LOGI(TAG, "event: VIEW_DATA_WIFI_LIST");

            wifi_list_init(); // clear and init

            lv_obj_clear_flag(ui_wifi_list, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_wifi_scan_wait, LV_OBJ_FLAG_HIDDEN);

            if (event_data == NULL) {
                // lv_obj_add_flag( ui_wifi_scan_wait, LV_OBJ_FLAG_HIDDEN );
                break;
            }
            struct view_data_wifi_list *p_list = (struct view_data_wifi_list *)event_data;

            bool have_password                 = true;

            if (p_list->is_connect) {
                create_wifi_item(ui_wifi_list, p_list->connect.ssid, p_list->connect.auth_mode, p_list->connect.ssid, true);
            }
            for (int i = 0; i < p_list->cnt; i++) {
                ESP_LOGI(TAG, "ssid:%s, rssi:%d, auth mode:%d", p_list->aps[i].ssid, p_list->aps[i].rssi, p_list->aps[i].auth_mode);
                if (p_list->is_connect) {
                    if (strcmp(p_list->aps[i].ssid, p_list->connect.ssid) != 0) {
                        create_wifi_item(ui_wifi_list, p_list->aps[i].ssid, p_list->aps[i].auth_mode, p_list->aps[i].rssi, false);
                    }
                } else {
                    create_wifi_item(ui_wifi_list, p_list->aps[i].ssid, p_list->aps[i].auth_mode, p_list->aps[i].rssi, false);
                }
            }
            break;
        }
        case VIEW_EVENT_WIFI_CONNECT_RET: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_CONNECT_RET");

            lv_obj_t *cur_screen = lv_scr_act();

            if (cur_screen != ui_screen_wifi) {
                break;
            }
            struct view_data_wifi_connet_ret_msg *p_data = (struct view_data_wifi_connet_ret_msg *)event_data;

            // update wifi list
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WIFI_LIST_REQ, NULL, 0, portMAX_DELAY);

            // show connect result
            ui_wifi_connect_ret_init(p_data);

            break;
        }
        case VIEW_EVENT_SENSOR_DATA: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_SENSOR_DATA");

            struct view_data_sensor_data *p_data = (struct view_data_sensor_data *)event_data;
            char                          data_buf[32];

            memset(data_buf, 0, sizeof(data_buf));
            switch (p_data->sensor_type) {
                case SENSOR_DATA_CO2: {
                    snprintf(data_buf, sizeof(data_buf), "%d", (int)p_data->vaule);
                    ESP_LOGI(TAG, "update co2:%s", data_buf);
                    lv_label_set_text(ui_co2_data, data_buf);
                    break;
                }
                case SENSOR_DATA_TVOC: {
                    snprintf(data_buf, sizeof(data_buf), "%d", (int)p_data->vaule);
                    ESP_LOGI(TAG, "update tvoc:%s", data_buf);
                    lv_label_set_text(ui_tvoc_data, data_buf);
                    break;
                }
                case SENSOR_DATA_TEMP: {
                    snprintf(data_buf, sizeof(data_buf), "%.1f", p_data->vaule);
                    ESP_LOGI(TAG, "update temp:%s", data_buf);
                    lv_label_set_text(ui_temp_data_2, data_buf);
                    break;
                }
                case SENSOR_DATA_HUMIDITY: {
                    snprintf(data_buf, sizeof(data_buf), "%d", (int)p_data->vaule);
                    ESP_LOGI(TAG, "update humidity:%s", data_buf);
                    lv_label_set_text(ui_humidity_data_2, data_buf);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case VIEW_EVENT_SCREEN_CTRL: {
            bool *p_st = (bool *)event_data;

            if (*p_st == 1) {
                lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
            } else {
                lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_COVER, 0);
                lv_obj_set_style_bg_color(lv_layer_top(), lv_color_black(), 0);
            }
            break;
        }
        default:
            break;
    }
    lv_port_sem_give();
}


int indicator_view_init(void)
{

    esp_event_loop_args_t view_event_task_args = {
        .queue_size      = 10,
        .task_name       = "view_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 10240,
        .task_core_id    = tskNO_AFFINITY,
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&view_event_task_args, &view_event_handle));

    ui_init();
    wifi_list_event_init();

    btn_time_save_init();
    zone_auto_update_cfg_init();
    ui_hour_some_init();

    for (int i = 0; i < VIEW_EVENT_ALL; i++) {
        ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                                 VIEW_EVENT_BASE, i,
                                                                 __view_event_handler, NULL, NULL));
    }

    iotex_view_cfg_event_register();
}
