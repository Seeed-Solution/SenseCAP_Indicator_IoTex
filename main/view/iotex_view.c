/**
 * @file iotex_view.c
 * @date  21 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */
#include "iotex_view.h"

static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    lv_port_sem_take();
    switch (id) {
        case VIEW_EVENT_MQTT_IOTEX_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_MQTT_IOTEX_CFG");
            w3b_cfg_interface *p_cfg = (w3b_cfg_interface *)event_data;
            lv_textarea_set_text(ui_TextArea_SN, p_cfg->sn);
            lv_textarea_set_text(ui_TextArea_WAD, p_cfg->wallet);
            break;
        }
        default:
            break;
    }
    lv_port_sem_give();
}

int iotex_view_init(void)
{
    int i = 0;
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_MQTT_IOTEX_CFG,
                                                             __view_event_handler, NULL, NULL));
}
