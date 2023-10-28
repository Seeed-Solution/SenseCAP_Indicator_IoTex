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
static W3B_CFG w3b_cfg;
static int status_now = -1;

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

static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    switch (id) {
        case CFG_EVENT_VIEW: {
            ESP_LOGI(TAG, "event: CFG_EVENT_VIEW");
			
            int len = sizeof(W3B_CFG); 
            if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
                ESP_LOGE(TAG, "cfg_read_fn failed");
                return;
            }
            
            break;
        }
    }
}

int8_t *iotex_devinfo_mac_get(enum dev_mac_type mac_type)
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

static void iotex_dev_status_handle(int iotex_status)
{
    switch (iotex_status)
    {
    case 0:
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SHOULD_ENROLL, NULL, NULL, portMAX_DELAY);
        break;
    case 1:        
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_CONFIRM_NEEDED, NULL, NULL, portMAX_DELAY);
        break;    
    case 2:        
        esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SUCCESS, NULL, NULL, portMAX_DELAY);
        break;           
    default:
        break;
    }
}

void iotex_upload_data_set_status(int status)
{
    if (status > status_now) {

        status_now = status;

        iotex_dev_status_handle(status_now);

    }
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

        iotex_dev_access_data_upload_with_userdata(user_data, 1, IOTEX_USER_DATA_TYPE_JSON, iotex_devinfo_mac_get(DEV_MAC_TYPE_HEX));

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

                sensor_data.type  = UPLOAD_DATA_CO2;
                sensor_data.value = (int)p_data->vaule;
                esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                break;
            }
            case SENSOR_DATA_TVOC: {

                sensor_data.type  = UPLOAD_DATA_TVOC;
                sensor_data.value = (int)p_data->vaule;
                esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                break;
            }
            case SENSOR_DATA_TEMP: {

                sensor_data.type  = UPLOAD_DATA_TEMP;
                sensor_data.value = (int)(p_data->vaule * 10);
                esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_UPSTREAM, &sensor_data, sizeof(sensor_data_t), portMAX_DELAY);

                break;
            }
            case SENSOR_DATA_HUMIDITY: {

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

void iotex_send_wallet_address_to_Seeed(char *buf, int32_t buf_len) 
{
    if ( buf == NULL || buf_len == 0)
        return;

    memset(w3b_cfg.cfg.wallet, 0, sizeof(w3b_cfg.cfg.wallet));
    memcpy(w3b_cfg.cfg.wallet, buf, buf_len);
    w3b_cfg.cfg.wallet_len = buf_len;

    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_WRITE, &w3b_cfg.cfg, sizeof(w3b_cfg_interface), portMAX_DELAY);
}

static eth_cfg eth_address;

void iotex_send_eth_address_to_Seeed(char *buf, int32_t buf_len) 
{
    if ( buf == NULL || buf_len == 0)
        return;

    memset(eth_address.eth, 0, sizeof(eth_address.eth));
    memcpy(eth_address.eth, buf, buf_len);
    eth_address.eth_len = buf_len;    

    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, DEVICE_ETH_EVENT_TRIGGER, &eth_address, sizeof(eth_cfg), portMAX_DELAY);
}

int iotex_devinfo_query_dev_sn(void)
{
    int len = sizeof(W3B_CFG); 

    if (0 == w3b_cfg.cfg.sn_len) {
        if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
            ESP_LOGE(TAG, "cfg_read_fn failed");
            return -1;
        }
    }

    if (0 == w3b_cfg.cfg.sn_len)
        return -2;

    printf("SN[%d] : %s\n", w3b_cfg.cfg.sn_len, w3b_cfg.cfg.sn);

    return 0;
}

int8_t *iotex_devinfo_dev_sn_get(void) 
{
    if (w3b_cfg.cfg.sn_len)
        return (int8_t *)w3b_cfg.cfg.sn;

    return NULL;
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

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(cfg_event_handle, 
                                                             CFG_EVENT_BASE, CFG_EVENT_VIEW, 
                                                             __view_event_handler, NULL, NULL));                                                             

    iotex_dev_mac_init();

}