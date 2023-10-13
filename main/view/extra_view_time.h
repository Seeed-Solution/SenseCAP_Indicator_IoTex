/**
 * @file extra_view_time.h
 * @date  12 October 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef EXTRA_VIEW_TIME_H
#define EXTRA_VIEW_TIME_H

#ifdef __cplusplus
extern "C" {
#endif
#include "indicator_view.h"

extern lv_obj_t *ui_time_zone;
extern lv_obj_t *ui_zone_auto_update_cfg;
extern lv_obj_t *ui_turn_off_after_time_cfg;
extern lv_obj_t *ui_time_save;
extern lv_obj_t *ui_hour;
extern lv_obj_t *ui_hour_dis;
extern lv_obj_t *ui_min;
extern lv_obj_t *ui_min_dis;

void time_zone_init();
void zone_auto_update_cfg_init();
void btn_time_save_init();
void ui_hour_some_init();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*EXTRA_VIEW_TIME_H*/