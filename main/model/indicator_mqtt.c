#include "indicator_mqtt.h"

static const char *TAG = "MQTT";

bool mqtt_net_flag     = false;

// instance_mqtt_container *instance_container;
// gloable func for get the mqtt flag
bool get_mqtt_net_flag(void)
{
    return mqtt_net_flag;
}
/**
 * @brief MQTT interface start port function.
 *
 * This function starts or restarts an MQTT client instance based on the provided flag.
 *
 * @param instance A pointer to the MQTT instance.
 * @param flag The MQTT start flag (MQTT_START or MQTT_RESTART).
 *
 * @note Use MQTT_RESTART when you need to change the MQTT broker URL, username, password, or other connection parameters.
 */
static void mqtt_start_interface(const instance_mqtt *instance, enum MQTT_START_ENUM flag)
{
    if (mqtt_net_flag == false) {
        return;
    }

    // Check for NULL instance or missing fields
    if(!instance)
    {
        ESP_LOGE(TAG, "instance is NULL, make sure you have initialized instance");
        return;
    }
    if(!instance->mqtt_name)
    {
        ESP_LOGE(TAG, "mqtt_name is NULL, make sure you have initialized instance");
        return;
    }
    if(!instance->mqtt_starter)
    {
        ESP_LOGE(TAG, "mqtt_starter is NULL, make sure you have initialized instance");
        return;
    }

    // Check for existing MQTT connection
    if (instance->mqtt_connected_flag) {
        switch (flag) {
            case MQTT_RESTART:
                esp_mqtt_client_stop(instance->mqtt_client);
                esp_mqtt_client_destroy(instance->mqtt_client);
                break;
            case MQTT_START:
                ESP_LOGW(TAG, "%s is already connected.", instance->mqtt_name);
                return;
            default:
                ESP_LOGE(TAG, "Invalid flag.");
                break;
        }
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
        case MQTT_APP_START: {
            ESP_LOGI(TAG, "event: MQTT_START");

            instance_mqtt_t *p_instance = (instance_mqtt_t *)event_data;
            instance_mqtt   *instance   = *p_instance;

            if (mqtt_net_flag && (instance->is_using)) {
                mqtt_start_interface(instance, MQTT_START);
            }

            break;
        }
        case MQTT_APP_RESTART: {
            ESP_LOGI(TAG, "event: MQTT_RESTART");
            instance_mqtt_t *p_instance = (instance_mqtt_t *)event_data;
            instance_mqtt   *instance   = *p_instance;
            if (mqtt_net_flag && (instance->is_using)) {
                mqtt_start_interface(instance, MQTT_RESTART);
            }
            break;
        }
    }
}


int indicator_mqtt_init(void)
{
    // instance_container = create_instance_mqtt_container();

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_WIFI_ST,
                                                             __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, WIFI_EVENT_STA_DISCONNECTED,
                                                             __view_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(mqtt_app_event_handle,
                                                             MQTT_APP_EVENT_BASE, MQTT_APP_START,
                                                             __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(mqtt_app_event_handle,
                                                             MQTT_APP_EVENT_BASE, MQTT_APP_RESTART,
                                                             __view_event_handler, NULL, NULL));
}


// unnessary function

void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}