#include "upload_data.h"
#include "wsiotsdk.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "view_data.h"

static const char *TAG           = "upload";

struct upload_data g_upload_data = {0};

static SemaphoreHandle_t __g_data_upload_sem;

struct dev_mac dev_mac_t;

static void hex2str(char *buf_hex, int len, char *str)
{
    int        i, j;
    const char hexmap[] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    for (i = 0, j = 0; i < len; i++) {
        str[j++] = hexmap[buf_hex[i] >> 4];
        str[j++] = hexmap[buf_hex[i] & 0x0F];
    }
    str[j] = 0;
}

static void iotex_dev_mac_init(void)
{

    memset(&dev_mac_t, 0, sizeof(struct dev_mac));
    esp_read_mac(dev_mac_t.mac, ESP_MAC_WIFI_STA);

    dev_mac_t.mac[5] += UPLOAD_DATA_TEST_DEV_MAC_OFFSET;

    hex2str(dev_mac_t.mac, 6, dev_mac_t.mac_str);
    dev_mac_t.valid = 1;

    printf("Dev MAC %s\n", dev_mac_t.mac_str);
}

int8_t *iotex_dev_mac_get(enum dev_mac_type mac_type)
{

    int8_t *mac_addr = NULL;

    if (0 == dev_mac_t.valid)
        return NULL;

    switch (mac_type) {
        case DEV_MAC_TYPE_HEX:
            mac_addr = dev_mac_t.mac;
            break;
        case DEV_MAC_TYPE_STR:
            mac_addr = dev_mac_t.mac_str;
            break;
        default:
            mac_addr = NULL;
            break;
    }

    return mac_addr;
}

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

            g_upload_data.temp = (float)value / 10.0;
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

        iotex_dev_access_data_upload_with_userdata(user_data, 1, IOTEX_USER_DATA_TYPE_JSON, iotex_dev_mac_get(DEV_MAC_TYPE_HEX));

        printf("Upload : [co2] : %d, [tvoc] : %d, [temp] : %.1f, [humidity] : %d\n", g_upload_data.co2, g_upload_data.tvoc, g_upload_data.temp, g_upload_data.humidity);

        cJSON_Delete(user_data);

        g_upload_data.valid_bit = 0;
    }
}

typedef struct sensor_data {
    int type;
    int value;
} sensor_data_t;

static void __upload_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    if (id == DATA_EVENT_SRNSOR_UPSTREAM) {
        iotex_upload_data_set_value(((sensor_data_t *)event_data)->value, ((sensor_data_t *)event_data)->type);
    }
}

static void __data_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    if (id == DATA_EVENT_SRNSOR_CAPTURE) {
        xSemaphoreTake(__g_data_upload_sem, portMAX_DELAY);
        sensor_data_t                 sensor_data = {0};
        struct view_data_sensor_data *p_data      = (struct view_data_sensor_data *)event_data;

        switch (p_data->sensor_type) {
            case SENSOR_DATA_CO2: {

                //                iotex_upload_data_set_value((int)p_data->vaule, UPLOAD_DATA_CO2);

                sensor_data.type  = UPLOAD_DATA_CO2;
                sensor_data.value = (int)p_data->vaule;
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
        /* out of switch*/
        xSemaphoreGive(__g_data_upload_sem);
    }
}

void iotex_upload_data_init(void)
{

    __g_data_upload_sem = xSemaphoreCreateBinary();
    if (__g_data_upload_sem != NULL)
        xSemaphoreGive(__g_data_upload_sem);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM,
                                                             __upload_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(model_event_handle,
                                                             DATA_EVENT_BASE, DATA_EVENT_SRNSOR_CAPTURE,
                                                             __data_event_handler, NULL, NULL));

    iotex_dev_mac_init();
}