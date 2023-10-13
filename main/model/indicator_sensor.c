#include "cobs.h"
#include "driver/uart.h"
#include "esp_timer.h"
#include "indicator_model.h"
#include "nvs.h"
#include "time.h"
#include <stdlib.h>

static const char *TAG = "sensor-model";

static SemaphoreHandle_t __g_data_mutex;

#define SENSOR_HISTORY_DATA_DEBUG         0
#define SENSOR_COMM_DEBUG                 0

#define ESP32_RP2040_TXD                  (19)
#define ESP32_RP2040_RXD                  (20)
#define ESP32_RP2040_RTS                  (UART_PIN_NO_CHANGE)
#define ESP32_RP2040_CTS                  (UART_PIN_NO_CHANGE)

#define ESP32_COMM_PORT_NUM               (2)
#define ESP32_COMM_BAUD_RATE              (115200)
#define ESP32_RP2040_COMM_TASK_STACK_SIZE (1024 * 4)
#define BUF_SIZE                          (512)

uint8_t buf[BUF_SIZE];  // recv
uint8_t data[BUF_SIZE]; // decode

enum pkt_type {

    PKT_TYPE_CMD_COLLECT_INTERVAL  = 0xA0, // uin32_t
    PKT_TYPE_CMD_BEEP_ON           = 0xA1, // uin32_t  ms: on time
    PKT_TYPE_CMD_BEEP_OFF          = 0xA2,
    PKT_TYPE_CMD_SHUTDOWN          = 0xA3, // uin32_t
    PKT_TYPE_CMD_POWER_ON          = 0xA4,

    PKT_TYPE_SENSOR_SCD41_TEMP     = 0xB0, // float
    PKT_TYPE_SENSOR_SCD41_HUMIDITY = 0xB1, // float
    PKT_TYPE_SENSOR_SCD41_CO2      = 0xB2, // float

    PKT_TYPE_SENSOR_SHT41_TEMP     = 0xB3, // float
    PKT_TYPE_SENSOR_SHT41_HUMIDITY = 0xB4, // float

    PKT_TYPE_SENSOR_TVOC_INDEX     = 0xB5, // float

};

static int __data_parse_handle(uint8_t *p_data, ssize_t len)
{
    struct view_data_sensor_data data;
    uint8_t                      pkt_type = p_data[0];
    memset(&data, 0, sizeof(data));
    switch (pkt_type) {
        case PKT_TYPE_SENSOR_SCD41_CO2: {
            if (len < (sizeof(data.vaule) + 1)) {
                break;
            }

            data.sensor_type = SENSOR_DATA_CO2;
            memcpy(&data.vaule, &p_data[1], sizeof(data.vaule));

            break;
        }

        case PKT_TYPE_SENSOR_SHT41_TEMP: {

            if (len < (sizeof(data.vaule) + 1)) {
                break;
            }

            data.sensor_type = SENSOR_DATA_TEMP;
            memcpy(&data.vaule, &p_data[1], sizeof(data.vaule));

            break;
        }

        case PKT_TYPE_SENSOR_SHT41_HUMIDITY: {
            if (len < (sizeof(data.vaule) + 1)) {
                break;
            }

            data.sensor_type = SENSOR_DATA_HUMIDITY;
            memcpy(&data.vaule, &p_data[1], sizeof(data.vaule));

            break;
        }

        case PKT_TYPE_SENSOR_TVOC_INDEX: {
            if (len < (sizeof(data.vaule) + 1)) {
                break;
            }

            data.sensor_type = SENSOR_DATA_TVOC;
            memcpy(&data.vaule, &p_data[1], sizeof(data.vaule));
            // __sensor_present_data_update(&__g_sensor_present_data.tvoc, data.vaule);

            // esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_SENSOR_DATA,
            //                   &data, sizeof(struct view_data_sensor_data), portMAX_DELAY);
            break;
        }

        default:
            return ESP_FAIL;
            break;
    }
    esp_event_post_to(model_event_handle, DATA_EVENT_BASE, DATA_EVENT_SRNSOR_CAPTURE,
                      &data, sizeof(struct view_data_sensor_data), portMAX_DELAY); // IMPORTANT
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_SENSOR_DATA,
                      &data, sizeof(struct view_data_sensor_data), portMAX_DELAY);
    return ESP_OK;
}

static int __cmd_send(uint8_t cmd, void *p_data, uint8_t len)
{
    uint8_t buf[32]  = {0};
    uint8_t data[32] = {0};

    if (len > 31) {
        return -1;
    }

    uint8_t index = 1;

    data[0]       = cmd;

    if (len > 0 && p_data != NULL) {
        memcpy(&data[1], p_data, len);
        index += len;
    }
    cobs_encode_result ret = cobs_encode(buf, sizeof(buf), data, index);
#if 1 // SENSOR_COMM_DEBUG
    ESP_LOGI(TAG, "encode status:%d, len:%d", ret.status, ret.out_len);
    for (int i = 0; i < ret.out_len; i++) {
        printf("0x%x ", buf[i]);
    }
    printf("\r\n");
#endif

    if (ret.status == COBS_ENCODE_OK) {
        return uart_write_bytes(ESP32_COMM_PORT_NUM, buf, ret.out_len + 1);
    }
    return -1;
}

static void esp32_rp2040_comm_task(void *arg)
{
    uart_config_t uart_config = {
        .baud_rate  = ESP32_COMM_BAUD_RATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(ESP32_COMM_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ESP32_COMM_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ESP32_COMM_PORT_NUM, ESP32_RP2040_TXD, ESP32_RP2040_RXD, ESP32_RP2040_RTS, ESP32_RP2040_CTS));

    __cmd_send(PKT_TYPE_CMD_POWER_ON, NULL, 0);
    cobs_decode_result ret;

    while (1) {
        int len = uart_read_bytes(ESP32_COMM_PORT_NUM, buf, (BUF_SIZE - 1), 1 / portTICK_PERIOD_MS);
#if SENSOR_COMM_DEBUG
        ESP_LOGI(TAG, "len:%d", len);
#endif
        int      index       = 0;
        uint8_t *p_buf_start = buf;
        uint8_t *p_buf_end   = buf;
        if (len > 0) {

#if SENSOR_COMM_DEBUG
            printf("recv:  ");
            for (int i = 0; i < len; i++) {
                printf("0x%x ", buf[i]);
            }
            printf("\r\n");
#endif
            while (p_buf_start < (buf + len)) {
                uint8_t *p_buf_end = p_buf_start;
                while (p_buf_end < (buf + len)) {
                    if (*p_buf_end == 0x00) {
                        break;
                    }
                    p_buf_end++;
                }
                // decode buf
                memset(data, 0, sizeof(data));
                ret = cobs_decode(data, sizeof(data), p_buf_start, p_buf_end - p_buf_start);

#if SENSOR_COMM_DEBUG
                ESP_LOGI(TAG, "decode status:%d, len:%d, type:0x%x  ", ret.status, ret.out_len, data[0]);
                printf("decode: ");
                for (int i = 0; i < ret.out_len; i++) {
                    printf("0x%x ", data[i]);
                }
                printf("\r\n");
#endif
                if (ret.out_len > 1 && ret.status == COBS_DECODE_OK) { // todo  ret.status
                    __data_parse_handle((uint8_t *)data, ret.out_len);
                }

                p_buf_start = p_buf_end + 1; // next message
            }
        }
    }
}

static void __sensor_shutdown(void)
{
    int ret = 0;
    ret     = __cmd_send(PKT_TYPE_CMD_SHUTDOWN, NULL, 0);
    if (ret <= 0) {
        ESP_LOGI(TAG, "sensor shutdown fail!. %d", ret);
    }
}

static void __view_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    // int i;
    switch (id) {
        case VIEW_EVENT_SHUTDOWN: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_SHUTDOWN");
            __sensor_shutdown();
            break;
        }
        default:
            break;
    }
}

int indicator_sensor_init(void)
{
    __g_data_mutex = xSemaphoreCreateMutex();

    xTaskCreate(esp32_rp2040_comm_task, "esp32_rp2040_comm_task", ESP32_RP2040_COMM_TASK_STACK_SIZE, NULL, 2, NULL);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle,
                                                             VIEW_EVENT_BASE, VIEW_EVENT_SHUTDOWN,
                                                             __view_event_handler, NULL, NULL));
}
