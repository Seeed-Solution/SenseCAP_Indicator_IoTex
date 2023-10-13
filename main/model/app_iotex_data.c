#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_random.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "esp_mac.h"
#include "nvs_flash.h"

#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "mqtt_client.h"
#include "wsiotsdk.h"

#include "ProtoBuf/user_data.pb.h"
#include "iotex_dev_access.h"
#include "iotex_dev_access_config.h" // Fill the token and mqtt topic obtained through w3bstream studio
#include "app_iotex_data.h"

#include "indicator_view.h"

static const char *TAG = "Data_IoTex";

/****************** Dumb & Upload Data ******************/

// #define DATA_EVENT_SRNSOR_UPSTREAM 0

struct upload_data g_upload_data = {0};

static SemaphoreHandle_t __g_data_upload_sem;

void iotex_upload_data_set_value(int value, enum upload_data_type data_type)
{

    int8_t mac[6] = {0};

    switch (data_type) {
        case UPLOAD_DATA_CO2:

            g_upload_data.co2 = value;
            g_upload_data.valid_bit |= BIT_UPLOAD_DATA_CO2;

            break;
        case UPLOAD_DATA_TVOC:

            g_upload_data.tvoc = value;
            g_upload_data.valid_bit |= BIT_UPLOAD_DATA_TVOC;

            break;
        case UPLOAD_DATA_TEMP:

            g_upload_data.temp = value;
            g_upload_data.valid_bit |= BIT_UPLOAD_DATA_TEMP;

            break;
        case UPLOAD_DATA_HUMIDITY:

            g_upload_data.humidity = value;
            g_upload_data.valid_bit |= BIT_UPLOAD_DATA_HUMIDITY;

            break;
        default:
            break;
    }

    if ((BIT_UPLOAD_DATA_READY == g_upload_data.valid_bit & BIT_UPLOAD_DATA_MASK)) {

        cJSON *user_data = cJSON_CreateObject();

        cJSON_AddNumberToObject(user_data, "co2", g_upload_data.co2);
        cJSON_AddNumberToObject(user_data, "tvoc", g_upload_data.tvoc);
        cJSON_AddNumberToObject(user_data, "temp", g_upload_data.temp);
        cJSON_AddNumberToObject(user_data, "humidity", g_upload_data.humidity);

        esp_read_mac(mac, ESP_MAC_WIFI_STA);

        iotex_dev_access_data_upload_with_userdata(user_data, 1, IOTEX_USER_DATA_TYPE_JSON, mac);

        cJSON_Delete(user_data);

        g_upload_data.valid_bit = 0;
    }
}

typedef struct sensor_data {
    int type;
    int value;
} sensor_data_t;

static void __data_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case DATA_EVENT_SRNSOR_CAPTURE: // catupre data from sensor
        {

            ESP_LOGI(TAG, "sensor data event: %d  value: %f", ((struct view_data_sensor_data *)event_data)->sensor_type, ((struct view_data_sensor_data *)event_data)->vaule);

            sensor_data_t sensor_data = {0};

            xSemaphoreTake(__g_data_upload_sem, portMAX_DELAY);

            struct view_data_sensor_data *p_data = (struct view_data_sensor_data *)event_data;

            switch (p_data->sensor_type) {
                case SENSOR_DATA_CO2: {

                    //                iotex_upload_data_set_value((int)p_data->vaule, UPLOAD_DATA_CO2);

                    sensor_data.type  = UPLOAD_DATA_CO2;
                    sensor_data.value = (int)p_data->vaule;
                    // esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);
                    esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                    break;
                }
                case SENSOR_DATA_TVOC: {

                    //                iotex_upload_data_set_value((int)p_data->vaule, UPLOAD_DATA_TVOC);

                    sensor_data.type  = UPLOAD_DATA_TVOC;
                    sensor_data.value = (int)p_data->vaule;
                    esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                    break;
                }
                case SENSOR_DATA_TEMP: {

                    //                iotex_upload_data_set_value((int)(p_data->vaule * 10), UPLOAD_DATA_TEMP);

                    sensor_data.type  = UPLOAD_DATA_TEMP;
                    sensor_data.value = (int)(p_data->vaule * 10);
                    esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                    break;
                }
                case SENSOR_DATA_HUMIDITY: {

                    //                iotex_upload_data_set_value((int)p_data->vaule, UPLOAD_DATA_HUMIDITY);

                    sensor_data.type  = UPLOAD_DATA_HUMIDITY;
                    sensor_data.value = (int)p_data->vaule;
                    esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                    break;
                }
                default:
                    break;
            }
            xSemaphoreGive(__g_data_upload_sem);
            break;
        }
        /* END OF DATA_EVENT_SRNSOR_CAPTURE */
        case DATA_EVENT_SRNSOR_UPSTREAM:
            iotex_upload_data_set_value(((sensor_data_t *)event_data)->value, ((sensor_data_t *)event_data)->type);
            break;
        /* END OF DATA_EVENT_SRNSOR_UPSTREAM */
        default:
            break;
    }
}

void iotex_upload_data_init(void)
{

    __g_data_upload_sem = xSemaphoreCreateBinary();
    if (__g_data_upload_sem != NULL)
        xSemaphoreGive(__g_data_upload_sem);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, DATA_EVENT_SRNSOR_CAPTURE,
                                                             __data_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM,
                                                             __data_event_handler, NULL, NULL));
}
