/**
 * @file iotex_event.h
 * @date  17 October 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */

#ifndef IOTEX_EVENT_H
#define IOTEX_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/* CFG & Data Event */
#include "model_data.h"
/**** Data ****/
// DATA_EVENT_SRNSOR_CAPTURE // Get Sensor data from model
// DATA_EVENT_SRNSOR_UPSTREAM // Upload sensor data to Portal

/**** CFG | Only SN? ****/
// CFG_EVENT_MODEL, // SN 的 获取
// BIND_EVENT_MODEL, // Bind flag 
// CFG_EVENT_WRITE // Write CFG to flash (which include wallet info)
esp_err_t bind_flag_write_fn(bool *flag);
/**
 * @brief 
 * 
 * @param flag 
 * @return esp_err_t 
 */
esp_err_t bind_flag_read_fn(bool *flag);
esp_err_t cfg_write_fn(W3B_CFG *cfg);
esp_err_t cfg_read_fn(W3B_CFG *cfg, int *len);

// CFG_EVENT_MODEL,
// BIND_EVENT_MODEL,
/* view Event */
#include "view_data.h"
// VIEW_EVENT_IOTEX_USER_CONFIRM 

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*IOTEX_EVENT_H*/