// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.3
// Project name: sensecap

#ifndef _SENSECAP_UI_H
#define _SENSECAP_UI_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
void up_Animation( lv_obj_t *TargetObject, int delay);
// SCREEN: ui_screen_binding
void ui_screen_binding_screen_init(void);
void ui_event_screen_binding( lv_event_t * e);
extern lv_obj_t *ui_screen_binding;
extern lv_obj_t *ui_time5;
void ui_event_wifi__st_button_4( lv_event_t * e);
extern lv_obj_t *ui_wifi__st_button_4;
extern lv_obj_t *ui_wifi_st_7;
extern lv_obj_t *ui_time4;
extern lv_obj_t *ui_scrolldots4;
extern lv_obj_t *ui_scrolldots5;
extern lv_obj_t *ui_scrolldots6;
extern lv_obj_t *ui_scrolldots7;
extern lv_obj_t *ui_Panel_user;
extern lv_obj_t *ui_Panel_SN;
extern lv_obj_t *ui_Label_SN;
extern lv_obj_t *ui_TextArea_SN;
extern lv_obj_t *ui_Label_WAD;
extern lv_obj_t *ui_TextArea_WAD;
extern lv_obj_t *ui_btn_bind;
extern lv_obj_t *ui_label_bind;
// SCREEN: ui_screen_setting
void ui_screen_setting_screen_init(void);
void ui_event_screen_setting( lv_event_t * e);
extern lv_obj_t *ui_screen_setting;
void ui_event_wifi__st_button_3( lv_event_t * e);
extern lv_obj_t *ui_wifi__st_button_3;
extern lv_obj_t *ui_wifi_st_3;
extern lv_obj_t *ui_time3;
extern lv_obj_t *ui_setting_icon;
extern lv_obj_t *ui_setting_title;
void ui_event_setting_wifi( lv_event_t * e);
extern lv_obj_t *ui_setting_wifi;
extern lv_obj_t *ui_setting_wifi_icon;
extern lv_obj_t *ui_setting_wifi_title;
void ui_event_setting_display( lv_event_t * e);
extern lv_obj_t *ui_setting_display;
extern lv_obj_t *ui_setting_display_icon;
extern lv_obj_t *ui_setting_display_title;
void ui_event_setting_time( lv_event_t * e);
extern lv_obj_t *ui_setting_time;
extern lv_obj_t *ui_setting_time_icon;
extern lv_obj_t *ui_setting_time_title;
extern lv_obj_t *ui_scrolldots3;
extern lv_obj_t *ui_scrolldots31;
extern lv_obj_t *ui_scrolldots32;
extern lv_obj_t *ui_scrolldots33;
void ui_event_setting_binding( lv_event_t * e);
extern lv_obj_t *ui_setting_binding;
extern lv_obj_t *ui_setting_binding_icon;
extern lv_obj_t *ui_setting_binding_title;
// SCREEN: ui_screen_wifi
void ui_screen_wifi_screen_init(void);
extern lv_obj_t *ui_screen_wifi;
extern lv_obj_t *ui_wifi_st_6;
extern lv_obj_t *ui_wifi_title;
void ui_event_back3( lv_event_t * e);
extern lv_obj_t *ui_back3;
// SCREEN: ui_screen_time
void ui_screen_time_screen_init(void);
void ui_event_screen_time( lv_event_t * e);
extern lv_obj_t *ui_screen_time;
void ui_event_wifi__st_button_1( lv_event_t * e);
extern lv_obj_t *ui_wifi__st_button_1;
extern lv_obj_t *ui_wifi_st_1;
extern lv_obj_t *ui_time1;
extern lv_obj_t *ui_date_panel;
extern lv_obj_t *ui_date;
extern lv_obj_t *ui_location;
extern lv_obj_t *ui_location_Icon;
extern lv_obj_t *ui_city;
extern lv_obj_t *ui_sensor_temp;
extern lv_obj_t *ui_temp_icon_1;
extern lv_obj_t *ui_temp_title_1;
extern lv_obj_t *ui_temp_unit_1;
extern lv_obj_t *ui_temp_data_1;
extern lv_obj_t *ui_sensor_humidity;
extern lv_obj_t *ui_humidity_icon_1;
extern lv_obj_t *ui_humidity_title_1;
extern lv_obj_t *ui_humidity_unit_1;
extern lv_obj_t *ui_humidity_data_1;
extern lv_obj_t *ui_scrolldots1;
extern lv_obj_t *ui_scrolldots11;
extern lv_obj_t *ui_scrolldots12;
extern lv_obj_t *ui_scrolldots13;
// SCREEN: ui_screen_sensor
void ui_screen_sensor_screen_init(void);
void ui_event_screen_sensor( lv_event_t * e);
extern lv_obj_t *ui_screen_sensor;
void ui_event_wifi__st_button_2( lv_event_t * e);
extern lv_obj_t *ui_wifi__st_button_2;
extern lv_obj_t *ui_wifi_st_2;
extern lv_obj_t *ui_time2;
extern lv_obj_t *ui_co2;
extern lv_obj_t *ui_co2_icon;
extern lv_obj_t *ui_co2_title;
extern lv_obj_t *ui_co2_data;
extern lv_obj_t *ui_co2_unit;
extern lv_obj_t *ui_tvoc_2;
extern lv_obj_t *ui_tvoc_icon_2;
extern lv_obj_t *ui_tvoc_title_2;
extern lv_obj_t *ui_tvoc_data;
extern lv_obj_t *ui_tvoc_unit_2;
extern lv_obj_t *ui_temp2;
extern lv_obj_t *ui_temp_icon_2;
extern lv_obj_t *ui_temp_title_2;
extern lv_obj_t *ui_temp_data_2;
extern lv_obj_t *ui_temp_unit_2;
extern lv_obj_t *ui_humidity2;
extern lv_obj_t *ui_humidity_icon_2;
extern lv_obj_t *ui_humidity_title_2;
extern lv_obj_t *ui_humidity_data_2;
extern lv_obj_t *ui_humidity_unit_2;
extern lv_obj_t *ui_scrolldots2;
extern lv_obj_t *ui_scrolldots21;
extern lv_obj_t *ui_scrolldots22;
extern lv_obj_t *ui_scrolldots23;
// SCREEN: ui_screen_display
void ui_screen_display_screen_init(void);
extern lv_obj_t *ui_screen_display;
extern lv_obj_t *ui_wifi_st_4;
void ui_event_back1( lv_event_t * e);
extern lv_obj_t *ui_back1;
extern lv_obj_t *ui_display_title;
extern lv_obj_t *ui_brighness;
extern lv_obj_t *ui_brighness_cfg;
extern lv_obj_t *ui_brighness_title;
extern lv_obj_t *ui_brighness_icon_1;
extern lv_obj_t *ui_brighness_icon_2;
extern lv_obj_t *ui_sleep_mode;
extern lv_obj_t *ui_sleep_mode_title;
void ui_event_sleep_mode_cfg( lv_event_t * e);
extern lv_obj_t *ui_sleep_mode_cfg;
extern lv_obj_t *ui_sleep_mode_time;
extern lv_obj_t *ui_after;
void ui_event_sleep_mode_time_cfg( lv_event_t * e);
extern lv_obj_t *ui_sleep_mode_time_cfg;
extern lv_obj_t *ui_min;
void ui_event_display_keyboard( lv_event_t * e);
extern lv_obj_t *ui_display_keyboard;
// SCREEN: ui_screen_date_time
void ui_screen_date_time_screen_init(void);
extern lv_obj_t *ui_screen_date_time;
extern lv_obj_t *ui_wifi_st_5;
void ui_event_back2( lv_event_t * e);
extern lv_obj_t *ui_back2;
extern lv_obj_t *ui_date_time_title;
extern lv_obj_t *ui_time_format;
extern lv_obj_t *ui_time_format_title;
extern lv_obj_t *ui_time_format_cfg;
extern lv_obj_t *ui_auto_update;
extern lv_obj_t *ui_auto_update_title;
void ui_event_auto_update_cfg( lv_event_t * e);
extern lv_obj_t *ui_auto_update_cfg;
extern lv_obj_t *ui_date_time;
extern lv_obj_t *ui_time_zone_title;
extern lv_obj_t *ui_time_zone_num_cfg;
extern lv_obj_t *ui_utc_tile;
extern lv_obj_t *ui_time_zone_sign_cfg_;
extern lv_obj_t *ui_daylight_title;
extern lv_obj_t *ui_daylight_cfg;
extern lv_obj_t *ui_manual_setting_title;
extern lv_obj_t *ui_date_cfg;
extern lv_obj_t *ui_hour_cfg;
extern lv_obj_t *ui_min_cfg;
extern lv_obj_t *ui_sec_cfg;
extern lv_obj_t *ui_time_label1;
extern lv_obj_t *ui_time_label2;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui_img_wifi_disconet_png);   // assets\wifi_disconet.png
LV_IMG_DECLARE( ui_img_setting_png);   // assets\setting.png
LV_IMG_DECLARE( ui_img_wifi_setting_png);   // assets\wifi_setting.png
LV_IMG_DECLARE( ui_img_display_png);   // assets\display.png
LV_IMG_DECLARE( ui_img_time_png);   // assets\time.png
LV_IMG_DECLARE( ui_img_1870666550);   // assets\we3stream logo (1).png
LV_IMG_DECLARE( ui_img_back_png);   // assets\back.png
LV_IMG_DECLARE( ui_img_lock_png);   // assets\lock.png
LV_IMG_DECLARE( ui_img_location_png);   // assets\location.png
LV_IMG_DECLARE( ui_img_temp_1_png);   // assets\temp_1.png
LV_IMG_DECLARE( ui_img_humidity_1_png);   // assets\humidity_1.png
LV_IMG_DECLARE( ui_img_co2_png);   // assets\co2.png
LV_IMG_DECLARE( ui_img_tvoc_png);   // assets\tvoc.png
LV_IMG_DECLARE( ui_img_temp_2_png);   // assets\temp_2.png
LV_IMG_DECLARE( ui_img_humidity_2_png);   // assets\humidity_2.png
LV_IMG_DECLARE( ui_img_high_light_png);   // assets\high_light.png
LV_IMG_DECLARE( ui_img_low_light_png);   // assets\low_light.png
LV_IMG_DECLARE( ui_img_location2_png);   // assets\location2.png
LV_IMG_DECLARE( ui_img_wifi_1_png);   // assets\wifi_1.png
LV_IMG_DECLARE( ui_img_wifi_2_png);   // assets\wifi_2.png
LV_IMG_DECLARE( ui_img_wifi_3_png);   // assets\wifi_3.png

LV_FONT_DECLARE( ui_font_font0);
LV_FONT_DECLARE( ui_font_font1);
LV_FONT_DECLARE( ui_font_font2);
LV_FONT_DECLARE( ui_font_font3);
LV_FONT_DECLARE( ui_font_font4);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
