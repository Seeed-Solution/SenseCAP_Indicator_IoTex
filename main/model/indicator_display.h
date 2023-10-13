#ifndef INDICATOR_DISPLAY_H
#define INDICATOR_DISPLAY_H

#include "indicator_model.h"
#include "indicator_view.h"

#ifdef __cplusplus
extern "C" {
#endif


int indicator_display_init(void);

bool indicator_display_st_get(void);

int indicator_display_on(void);

int indicator_display_off(void);

int indicator_display_sleep_restart(void);

#ifdef __cplusplus
}
#endif

#endif
