#include "indicator_mqtt.h"

#define X(name, str) str,
const char *MQTT_PLATFORM_STR[] = {
    MQTT_PLATFORM};
#undef X

static const char            *TAG                             = "MQTT";
bool                          mqtt_net_flag                   = false;
// static const instance_mqtt_t *instance_ptr[MQTT_PLATFORM_MAX] = {&mqtt_portal_instance, &mqtt_jetson_instance};
static const instance_mqtt_t *instance_ptr[MQTT_PLATFORM_MAX] = {};


// When use RESTART? you change the url, username, password, or other parameters, you need to restart mqtt
void mqtt_start_interface(instance_mqtt_t *instance, enum MQTT_START_ENUM flag)
{
    if (mqtt_net_flag == false) {
        return;
    }

    if (instance->mqtt_connected_flag && MQTT_START) { // if mqtt is connected, and not restart, return void
        return;
    } else if (instance->mqtt_connected_flag && MQTT_RESTART) {
        esp_mqtt_client_stop(instance->mqtt_client);
        esp_mqtt_client_destroy(instance->mqtt_client);
    }

    if (instance->mqtt_name == NULL) { // if mqtt_cfg is NULL, return void
        ESP_LOGE(TAG, "mqtt_name is NULL, which means instance is not initialized");
        return;
    }
    // first connecntion and reconnect
    instance->mqtt_starter(instance);
}


static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case VIEW_EVENT_WIFI_ST: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            struct view_data_wifi_st *p_st = (struct view_data_wifi_st *)event_data;
            if (p_st->is_network) { // avoid repeatly start mqtt
                if (mqtt_net_flag == false) {
                    mqtt_net_flag = true; // WiFI Network is connected
                    // for (int i = 0; i < MQTT_PLATFORM_MAX; i++) {
                    //     if (instance_ptr[i]->is_using) {
                    //         ESP_LOGE(TAG, "MQTT %s is Starting", MQTT_PLATFORM_STR[i]);
                    //         mqtt_start_interface(instance_ptr[i], MQTT_START);
                    //     }
                    // }VIEW_EVENT_WIFI_ST
                }
            } else {
                mqtt_net_flag = false;
            }
            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED: {
            mqtt_net_flag = false;
            break;
        }
    }
}


int indicator_mqtt_init(void)
{
    // indicator_mqtt_portal_init(&mqtt_portal_instance);
    // indicator_mqtt_jetson_init(&mqtt_jetson_instance);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_WIFI_ST,
                                                             __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, WIFI_EVENT_STA_DISCONNECTED,
                                                             __view_event_handler, NULL, NULL));
}


// unnessary function

void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}
