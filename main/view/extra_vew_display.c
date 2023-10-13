#include "extra_view_display.h"
lv_obj_t *ui_screen_always_on_cfg;
lv_obj_t *ui_turn_off_after_time;
lv_obj_t *ui_turn_off_after_time_cfg;
lv_obj_t *ui_event_screen_always_on_cfg;
lv_obj_t *ui_screen_always_on;
lv_obj_t *ui_screen_always_on_title;

void extra_ui_screen_display_screen_init(void)
{
    ui_screen_always_on = lv_obj_create(ui_screen_display);
    lv_obj_set_width(ui_screen_always_on, 400);
    lv_obj_set_height(ui_screen_always_on, 50);
    lv_obj_set_x(ui_screen_always_on, 0);
    lv_obj_set_y(ui_screen_always_on, 5);
    lv_obj_set_align(ui_screen_always_on, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_screen_always_on, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_screen_always_on_title = lv_label_create(ui_screen_always_on);
    lv_obj_set_width(ui_screen_always_on_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_screen_always_on_title, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_screen_always_on_title, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_screen_always_on_title, "Always-on");

    ui_screen_always_on_cfg = lv_switch_create(ui_screen_always_on);
    lv_obj_set_width(ui_screen_always_on_cfg, 50);
    lv_obj_set_height(ui_screen_always_on_cfg, 25);
    lv_obj_set_align(ui_screen_always_on_cfg, LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_bg_color(ui_screen_always_on_cfg, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_screen_always_on_cfg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_screen_always_on_cfg, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_screen_always_on_cfg, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(ui_screen_always_on_cfg, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_screen_always_on_cfg, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    ui_turn_off_after_time = lv_obj_create(ui_screen_display);
    lv_obj_set_width(ui_turn_off_after_time, 400);
    lv_obj_set_height(ui_turn_off_after_time, 50);
    lv_obj_set_x(ui_turn_off_after_time, 0);
    lv_obj_set_y(ui_turn_off_after_time, 65);
    lv_obj_set_align(ui_turn_off_after_time, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_turn_off_after_time, LV_OBJ_FLAG_HIDDEN);       /// Flags
    lv_obj_clear_flag(ui_turn_off_after_time, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_after = lv_label_create(ui_turn_off_after_time);
    lv_obj_set_width(ui_after, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_after, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_after, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_after, "Turn Off Screen After");

    ui_turn_off_after_time_cfg = lv_textarea_create(ui_turn_off_after_time);
    lv_obj_set_width(ui_turn_off_after_time_cfg, 60);
    lv_obj_set_height(ui_turn_off_after_time_cfg, 40);
    lv_obj_set_x(ui_turn_off_after_time_cfg, -50);
    lv_obj_set_y(ui_turn_off_after_time_cfg, 0);
    lv_obj_set_align(ui_turn_off_after_time_cfg, LV_ALIGN_RIGHT_MID);
    lv_textarea_set_placeholder_text(ui_turn_off_after_time_cfg, "1");
    lv_obj_set_style_bg_color(ui_turn_off_after_time_cfg, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_turn_off_after_time_cfg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_display_min = lv_label_create(ui_turn_off_after_time);
    lv_obj_set_width(ui_display_min, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_display_min, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_display_min, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_display_min, "min");

    lv_obj_add_event_cb(ui_screen_always_on_cfg, ui_event_screen_always_on_cfg, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_turn_off_after_time_cfg, ui_event_sleep_mode_time_cfg, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(ui_display_keyboard, ui_turn_off_after_time_cfg);
}