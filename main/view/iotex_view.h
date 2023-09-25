/**
 * @file iotex_view.h
 * @date  21 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef IOTEX_VIEW_H
#define IOTEX_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "indicator_view.h"

int iotex_view_init(void);
void show_sn_wallet(void);
void show_bind_state(void);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*IOTEX_VIEW_H*/