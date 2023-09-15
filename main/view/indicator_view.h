#ifndef INDICATOR_VIEW_H
#define INDICATOR_VIEW_H

#include "lvgl.h"
#include "ui.h"
#include "ui_helpers.h"
#include "view_data.h"

#ifdef __cplusplus
extern "C" {
#endif
LV_IMG_DECLARE( ui_img_lock_png );
int indicator_view_init( void );

#ifdef __cplusplus
}
#endif

#endif
