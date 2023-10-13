/**
 * @file extra_view_display.h
 * @date  12 October 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef EXTRA_VIEW_DISPLAY_H
#define EXTRA_VIEW_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "indicator_view.h"
extern lv_obj_t *ui_screen_always_on_cfg;
extern lv_obj_t *ui_event_screen_always_on_cfg;

extern lv_obj_t *ui_turn_off_after_time;
extern lv_obj_t *ui_turn_off_after_time_cfg;
extern lv_obj_t *ui_screen_always_on;
extern lv_obj_t *ui_screen_always_on_title;

void extra_ui_screen_display_screen_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*EXTRA_VIEW_DISPLAY_H*/