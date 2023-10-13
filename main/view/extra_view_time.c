#include "extra_view_time.h"

lv_obj_t *ui_time_zone;

void time_zone_init()
{
    ui_time_zone = lv_obj_create(ui_screen_date_time); // TODO verify what is configlist
    lv_obj_set_width(ui_time_zone, 400);
    lv_obj_set_height(ui_time_zone, 100);
    // lv_obj_set_x( ui_time_zone, 0 );
    // lv_obj_set_y( ui_time_zone, 88 );
    lv_obj_set_align(ui_time_zone, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_time_zone, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ONE); /// Flags
    lv_obj_clear_flag(ui_time_zone, LV_OBJ_FLAG_SCROLLABLE);                       /// Flags

    ui_time_zone_title = lv_label_create(ui_time_zone);
    lv_obj_set_width(ui_time_zone_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_time_zone_title, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_time_zone_title, 0);
    lv_obj_set_y(ui_time_zone_title, -20);
    lv_obj_set_align(ui_time_zone_title, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_time_zone_title, "Time Zone");

    ui_time_zone_num_cfg = lv_dropdown_create(ui_time_zone);
    lv_dropdown_set_options(ui_time_zone_num_cfg, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12");
    lv_obj_set_width(ui_time_zone_num_cfg, 69);
    lv_obj_set_height(ui_time_zone_num_cfg, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_time_zone_num_cfg, 150);
    lv_obj_set_y(ui_time_zone_num_cfg, -20);
    lv_obj_set_align(ui_time_zone_num_cfg, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_time_zone_num_cfg, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_bg_color(ui_time_zone_num_cfg, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_time_zone_num_cfg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_utc_tile = lv_label_create(ui_time_zone);
    lv_obj_set_width(ui_utc_tile, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_utc_tile, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_utc_tile, 30);
    lv_obj_set_y(ui_utc_tile, -20);
    lv_obj_set_align(ui_utc_tile, LV_ALIGN_CENTER);
    lv_label_set_text(ui_utc_tile, "UTC");

    ui_time_zone_sign_cfg_ = lv_dropdown_create(ui_time_zone);
    lv_dropdown_set_options(ui_time_zone_sign_cfg_, "+\n-");
    lv_obj_set_width(ui_time_zone_sign_cfg_, 50);
    lv_obj_set_height(ui_time_zone_sign_cfg_, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_time_zone_sign_cfg_, 84);
    lv_obj_set_y(ui_time_zone_sign_cfg_, -20);
    lv_obj_set_align(ui_time_zone_sign_cfg_, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_time_zone_sign_cfg_, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_bg_color(ui_time_zone_sign_cfg_, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_time_zone_sign_cfg_, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_daylight_title = lv_label_create(ui_time_zone);
    lv_obj_set_width(ui_daylight_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_daylight_title, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_daylight_title, 0);
    lv_obj_set_y(ui_daylight_title, 20);
    lv_obj_set_align(ui_daylight_title, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_daylight_title, "Daylight Saving Time");

    ui_daylight_cfg = lv_switch_create(ui_time_zone);
    lv_obj_set_width(ui_daylight_cfg, 50);
    lv_obj_set_height(ui_daylight_cfg, 25);
    lv_obj_set_x(ui_daylight_cfg, 0);
    lv_obj_set_y(ui_daylight_cfg, 20);
    lv_obj_set_align(ui_daylight_cfg, LV_ALIGN_RIGHT_MID);
    lv_obj_add_state(ui_daylight_cfg, LV_STATE_CHECKED); /// States
    lv_obj_set_style_bg_color(ui_daylight_cfg, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_daylight_cfg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_daylight_cfg, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_daylight_cfg, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(ui_daylight_cfg, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_daylight_cfg, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
}

lv_obj_t *ui_zone_auto_update_cfg;


void zone_auto_update_cfg_init()
{
    lv_obj_t *ui_zone_auto_update = lv_obj_create(ui_screen_date_time);
    lv_obj_set_width(ui_zone_auto_update, 400);
    lv_obj_set_height(ui_zone_auto_update, 50);
    // lv_obj_set_x( ui_auto_update, 0 );
    // lv_obj_set_y( ui_auto_update, -60 );
    lv_obj_set_align(ui_zone_auto_update, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_zone_auto_update, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_flag(ui_zone_auto_update, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *ui_zone_auto_update_title = lv_label_create(ui_zone_auto_update);
    lv_obj_set_width(ui_zone_auto_update_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_zone_auto_update_title, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_zone_auto_update_title, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_zone_auto_update_title, "Zone Auto Update");

    ui_zone_auto_update_cfg = lv_switch_create(ui_zone_auto_update);
    lv_obj_set_width(ui_zone_auto_update_cfg, 50);
    lv_obj_set_height(ui_zone_auto_update_cfg, 25);
    lv_obj_set_align(ui_zone_auto_update_cfg, LV_ALIGN_RIGHT_MID);
    lv_obj_add_state(ui_zone_auto_update_cfg, LV_STATE_CHECKED); /// States
    lv_obj_set_style_bg_color(ui_zone_auto_update_cfg, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_zone_auto_update_cfg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_zone_auto_update_cfg, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_zone_auto_update_cfg, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(ui_zone_auto_update_cfg, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_zone_auto_update_cfg, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
}

lv_obj_t *ui_time_save;

void btn_time_save_init()
{
    ui_time_save = lv_btn_create(ui_date_time);
    lv_obj_set_x(ui_time_save, 230);
    lv_obj_set_align(ui_time_save, LV_ALIGN_CENTER);
    lv_obj_set_width(ui_time_save, 50);
    lv_obj_set_height(ui_time_save, 40);
    lv_obj_clear_flag(ui_time_save, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(ui_time_save, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_time_save_label = lv_label_create(ui_time_save);
    lv_obj_set_align(ui_time_save_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_time_save_label, "Save");
}

lv_obj_t *ui_hour;
lv_obj_t *ui_hour_dis;
lv_obj_t *ui_min;
lv_obj_t *ui_min_dis;

void ui_hour_some_init()
{
    ui_hour = lv_obj_create(ui_screen_time);
    lv_obj_set_width(ui_hour, 200);
    lv_obj_set_height(ui_hour, 170);
    lv_obj_set_x(ui_hour, -110);
    lv_obj_set_y(ui_hour, 0);
    lv_obj_set_align(ui_hour, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_hour, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_hour, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_hour_dis = lv_label_create(ui_hour);
    lv_obj_set_width(ui_hour_dis, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_hour_dis, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_hour_dis, LV_ALIGN_CENTER);
    lv_label_set_text(ui_hour_dis, "00");
    lv_obj_set_style_text_font(ui_hour_dis, &ui_font_font4, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_min = lv_obj_create(ui_screen_time);
    lv_obj_set_width(ui_min, 200);
    lv_obj_set_height(ui_min, 170);
    lv_obj_set_x(ui_min, 110);
    lv_obj_set_y(ui_min, 0);
    lv_obj_set_align(ui_min, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_min, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_min, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_min, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_min_dis = lv_label_create(ui_min);
    lv_obj_set_width(ui_min_dis, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_min_dis, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_min_dis, LV_ALIGN_CENTER);
    lv_label_set_text(ui_min_dis, "00");
    lv_obj_set_style_text_font(ui_min_dis, &ui_font_font4, LV_PART_MAIN | LV_STATE_DEFAULT);
}