/**
 * @file indicator_mqtt_iotex.c
 * @date  18 September 2023

 * @author Spencer Yan
 *
 * @note Description of the file
 *
 * @copyright © 2023, Seeed Studio
 */
#include "app_mqtt_iotex.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_random.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_wifi.h"

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

static const char *TAG = "MQTT_IoTex";

#define KEY_BITS                      256
// #define IOTEX_DEBUG_ENABLE
// #define IOTEX_DEBUG_ENABLE_EXT

#define IOTEX_SIGNKEY_USE_NONE        0x00
#define IOTEX_SIGNKEY_USE_STATIC_DATA 0x01
#define IOTEX_SIGNKEY_USE_FLASH       0x02 // Not Support
#define IOTEX_SIGNKEY_USE_PRNG        0x04

#define IOTEX_SIGNKEY_USE_MODE        IOTEX_SIGNKEY_USE_STATIC_DATA

#define IOTEX_SIGNKEY_ECC_MODE        PSA_ECC_FAMILY_SECP_K1

#if (IOTEX_SIGNKEY_USE_MODE == IOTEX_SIGNKEY_USE_STATIC_DATA)
static const uint8_t private_key[] = {0xa1, 0x73, 0x6f, 0xbf, 0x37, 0xa2, 0xfc, 0xb8, 0xfe, 0xe2, 0x02, 0xdb, 0x0c, 0x63, 0x91, 0xdf, 0xa4, 0x61, 0x86, 0x29, 0xb1, 0x86, 0xa6, 0x90, 0x65, 0x85, 0x2d, 0xfc, 0xd8, 0x8f, 0x58, 0x19};
#endif

#if (IOTEX_SIGNKEY_USE_MODE == IOTEX_SIGNKEY_USE_PRNG)
#define IOTEX_SEED_USER_DEFINE 69834
#endif

psa_key_id_t key_id = 0;
uint8_t      exported[PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(KEY_BITS)];
uint32_t     exported_len;

/****************** MQTT Configutation ******************/

static void _mqtt_iotex_start(instance_mqtt *instance)
{
    if (get_mqtt_net_flag() == false) {
        return;
    }

    if (instance->mqtt_cfg != NULL) {
        free(instance->mqtt_cfg);
    }

    instance->mqtt_cfg  = (esp_mqtt_client_config_t *)malloc(sizeof(esp_mqtt_client_config_t));
    *instance->mqtt_cfg = (esp_mqtt_client_config_t){
        .broker.address.uri = iotex_dev_access_get_mqtt_connect_addr_in_format(),
        // .credentials.client_id               = Client_ID,
        // .credentials.username                = username,
        // .credentials.authentication.password = password,
    };
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "| Broker Address               | %-40s |", iotex_dev_access_get_mqtt_connect_addr_in_format());
    // ESP_LOGI(TAG, "| Client ID                    | %-40s |", Client_ID);
    // ESP_LOGI(TAG, "| username                     | %-40s |", username);
    // ESP_LOGI(TAG, "| password                     | %-40s |", password);

    instance->mqtt_client = esp_mqtt_client_init(instance->mqtt_cfg);
    esp_mqtt_client_register_event(instance->mqtt_client, ESP_EVENT_ANY_ID, instance->mqtt_event_handler, NULL);
    esp_mqtt_client_start(instance->mqtt_client);
}

/*Instance*/
instance_mqtt          mqtt_iotex_instance; // global entrance
static instance_mqtt_t instance_ptr = &mqtt_iotex_instance;

/****************** Declarations ******************/

static void _mqtt_iotex_start(instance_mqtt *instance);
static void _mqtt_iotex_init(instance_mqtt *instance);
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

// IoTex SDK Callbacks Declarations
time_t iotex_time_set_func(void);
int    iotex_mqtt_pubscription(unsigned char *topic, unsigned char *buf, unsigned int buflen, int qos);
int    iotex_mqtt_subscription(unsigned char *topic);
#if IOTEX_USE_SIGN_FUNC_EXT
int iotex_sign_message_func(const uint8_t *input, size_t input_length, uint8_t *signature, size_t *signature_length);
#endif

// examples functions
void iotex_devnet_upload_data_example_json(void);
void iotex_devnet_upload_data_example_pb(void);
void iotex_devnet_upload_data_example_raw(void);
void iotex_import_key_example(void);

/****************** Callbacks Definitions ******************/

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t  event  = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int                      msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {

        case MQTT_EVENT_CONNECTED: {
            instance_ptr->mqtt_connected_flag = true;

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
#if 0
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
#endif

            msg_id = esp_mqtt_client_subscribe(client, IOTEX_MQTT_SUB_TOPIC_DEFAULT, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            // ESP_LOGI(TAG, "| Subscribed Topics | %-8s | %-23.*s | msg_id | %d |", "Jetson", strlen(jetson_topic), jetson_topic, msg_id);

            iotex_dev_access_set_mqtt_status(IOTEX_MQTT_CONNECTED);

            break;
        }
        case MQTT_EVENT_DATA: {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            // // Queue is not full
            // if (uxQueueSpacesAvailable(mqtt_jetson_data_queue) > 0) {
            //     mqtt_data_t received_data;
            //     memcpy(received_data.topic, event->topic, event->topic_len);
            //     received_data.topic[event->topic_len] = '\0';
            //     memcpy(received_data.data, event->data, event->data_len);
            //     received_data.data[event->data_len] = '\0';
            //     if (xQueueSend(mqtt_jetson_data_queue, &received_data, portMAX_DELAY) != pdTRUE) {
            //         ESP_LOGE(TAG, "MQTT Jetson Data Queue send failed");
            //     }
            // }
            break;
        }
        case MQTT_EVENT_DISCONNECTED:
            instance_ptr->mqtt_connected_flag = false;

            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            iotex_dev_access_set_mqtt_status(IOTEX_MQTT_DISCONNECTED);
            break;
        case MQTT_EVENT_SUBSCRIBED:
#if 0
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
#endif
            iotex_dev_access_set_mqtt_status(IOTEX_MQTT_SUB_COMPLATED);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            // ESP_LOGI( TAG, "Other event id:%d", event->event_id );
            break;
    }
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

/*
 * If 'NTP over DHCP' is enabled, we set dynamic pool address
 * as a 'secondary' server. It will act as a fallback server in case that address
 * provided via NTP over DHCP is not accessible
 */
#if LWIP_DHCP_GET_NTP_SRV && SNTP_MAX_SERVERS > 1
    sntp_setservername(1, "pool.ntp.org");

#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2 // statically assigned IPv6 address is also possible
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) { // ipv6 ntp source "ntp.netnod.se"
        sntp_setserver(2, &ip6);
    }
#endif /* LWIP_IPV6 */

#else /* LWIP_DHCP_GET_NTP_SRV && (SNTP_MAX_SERVERS > 1) */
    // otherwise, use DNS address from a pool
    // sntp_setservername(0, CONFIG_SNTP_TIME_SERVER);

    // sntp_setservername(1, "time.windows.com");     // set the secondary NTP server (will be used only if SNTP_MAX_SERVERS > 1)
    sntp_setservername(0, "pool.ntp.org");
    sntp_setservername(1, "cn.ntp.org.cn");
#endif

    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();

    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i) {
        if (sntp_getservername(i)) {
            ESP_LOGI(TAG, "server %d: %s", i, sntp_getservername(i));
        } else {
            // we have either IPv4 or IPv6 address, let's print it
            char             buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}

static void obtain_time(void)
{
    /**
     * NTP server address could be aquired via DHCP,
     * see following menuconfig options:
     * 'LWIP_DHCP_GET_NTP_SRV' - enable STNP over DHCP
     * 'LWIP_SNTP_DEBUG' - enable debugging messages
     *
     * NOTE: This call should be made BEFORE esp aquires IP address from DHCP,
     * otherwise NTP option would be rejected by default.
     */
#if LWIP_DHCP_GET_NTP_SRV
    sntp_servermode_dhcp(1); // accept NTP offers from DHCP server, if any
#endif

    initialize_sntp();

    // wait for time to be set
    time_t    now         = 0;
    struct tm timeinfo    = {0};
    int       retry       = 0;
    const int retry_count = 15;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

time_t iotex_time_set_func(void)
{
    return time(NULL);
}

int iotex_mqtt_pubscription(unsigned char *topic, unsigned char *buf, unsigned int buflen, int qos)
{
    return esp_mqtt_client_publish(instance_ptr->mqtt_client, (const char *)topic, (const char *)buf, buflen, 1, 0);
}

int iotex_mqtt_subscription(unsigned char *topic)
{
    return esp_mqtt_client_subscribe(instance_ptr->mqtt_client, (const char *)topic, 1);
}

#if IOTEX_USE_SIGN_FUNC_EXT
int iotex_sign_message_func(const uint8_t *input, size_t input_length, uint8_t *signature, size_t *signature_length)
{
    return psa_sign_message(key_id, PSA_ALG_ECDSA(PSA_ALG_SHA_256), input, input_length, signature, 64, signature_length);
}
#endif

/**
 * @brief generate a key pair and export public key and private key
 *
 * @param exported_key
 * @param key_len
 */
void iotex_generate_signkey(unsigned char *exported_key, unsigned int *key_len)
{
    psa_status_t         status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

    uint8_t exported[PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(KEY_BITS)];
    size_t  exported_length = 0;

    printf("Generate a key pair...\n");

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_EXPORT);
    psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(IOTEX_SIGNKEY_ECC_MODE));
    psa_set_key_bits(&attributes, KEY_BITS);

    status = psa_generate_key(&attributes, &key_id);
    if (status != PSA_SUCCESS) {
        printf("Failed to generate key %d\n", status);
        return;
    }

#ifdef IOTEX_DEBUG_ENABLE
    printf("Success to generate a key pair: key id : %x\n", key_id);
#endif

    status = psa_export_key(key_id, exported_key, 32, &exported_length);
    if (status != PSA_SUCCESS) {
        printf("Failed to export pair key %d\n", status);
        return;
    }

#ifdef IOTEX_DEBUG_ENABLE
    printf("Exported a pair key len %d\n", exported_length);
#endif

    *key_len = exported_length;

#ifdef IOTEX_DEBUG_ENABLE
    for (int i = 0; i < exported_length; i++) {
        printf("%02x ", exported_key[i]);
    }
    printf("\n");
#endif

    status = psa_export_public_key(key_id, exported, sizeof(exported), &exported_length);
    if (status != PSA_SUCCESS) {
        printf("Failed to export public key %d\n", status);
        return;
    }
#ifdef IOTEX_DEBUG_ENABLE
    printf("Exported a public key len %d\n", exported_length);

    for (int i = 0; i < exported_length; i++) {
        printf("%02x ", exported[i]);
    }
    printf("\n");
#endif
}

/****************** IoTex Application ******************/

void iotex_import_key_example(void)
{
    psa_status_t         status;
    psa_key_attributes_t attributes       = PSA_KEY_ATTRIBUTES_INIT;
    unsigned char        prikey[32]       = {0};
    char                 dev_address[100] = {0};

    uint8_t key_mode                      = 0;

#if (IOTEX_SIGNKEY_USE_MODE == IOTEX_SIGNKEY_USE_STATIC_DATA)

    memcpy(prikey, private_key, sizeof(prikey));
    key_mode = 1;

#endif

#if (IOTEX_SIGNKEY_USE_MODE == IOTEX_SIGNKEY_USE_PRNG)

    unsigned int prikey_len = 0;

    extern void default_SetSeed(unsigned int seed);
    default_SetSeed(IOTEX_SEED_USER_DEFINE);
    iotex_generate_signkey(prikey, &prikey_len);

    key_mode = 0;
#endif

    if (key_mode) {

        /* Set key attributes */
        psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
        psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
        psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(IOTEX_SIGNKEY_ECC_MODE));
        psa_set_key_bits(&attributes, 256);

        /* Import the key */
        status = psa_import_key(&attributes, prikey, 32, &key_id);
        if (status != PSA_SUCCESS) {
#ifdef IOTEX_DEBUG_ENABLE
            printf("Failed to import pri key err %d\n", status);
#endif
            key_id = 0;

            return;
        }
#ifdef IOTEX_DEBUG_ENABLE
        else
            printf("Success to import pri key keyid %x\n", key_id);
#endif
    }

    status = psa_export_public_key(key_id, exported, sizeof(exported), (size_t *)&exported_len);
    if (status != PSA_SUCCESS) {
#ifdef IOTEX_DEBUG_ENABLE
        printf("Failed to export public key %d\n", status);
#endif
        return;
    }

#ifdef IOTEX_DEBUG_ENABLE
    printf("Exported a public key len %d\n", exported_len);
    for (int i = 0; i < exported_len; i++) {
        printf("%02x ", exported[i]);
    }
    printf("\n");
#endif

    iotex_dev_access_generate_dev_addr((const unsigned char *)exported, dev_address);
    printf("Dev_addr : %s\n", dev_address);

#ifdef IOTEX_DEBUG_ENABLE_EXT
    unsigned char inbuf[] = "hello devnet";
    unsigned char buf[65] = {0};
    unsigned int  sinlen  = 0;

    status                = psa_sign_message(key_id, PSA_ALG_ECDSA(PSA_ALG_SHA_256), inbuf, strlen((const char *)inbuf), (unsigned char *)buf, 65, &sinlen);
    if (status != PSA_SUCCESS) {
        printf("Failed to sign message %d\n", status);
    } else {
        printf("Success to sign message %d\n", sinlen);
    }

#ifdef IOTEX_DEBUG_ENABLE
    printf("Exported a sign len %d\n", sinlen);
    for (int i = 0; i < sinlen; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
#endif

    status = psa_verify_message(key_id, PSA_ALG_ECDSA(PSA_ALG_SHA_256), inbuf, strlen((const char *)inbuf), (unsigned char *)buf, sinlen);
    if (status != PSA_SUCCESS) {
        printf("Failed to verify message %d\n", status);
    } else {
        printf("Success to verify message\n");
    }
#endif
}

typedef struct __packed user_data {
    int   i;
    float f;
    bool  b;
} user_data_t;

void iotex_devnet_upload_data_example_raw(void)
{
    if (instance_ptr->mqtt_connected_flag == false) {
        return;
    }
    user_data_t  user_data;
    unsigned int len = sizeof(user_data);

    user_data.i      = 64;
    user_data.f      = 128.128;
    user_data.b      = true;

    iotex_dev_access_data_upload_with_userdata((void *)&user_data, len, IOTEX_USER_DATA_TYPE_RAW);
}

void iotex_devnet_upload_data_example_json(void)
{
    // Check if mqtt connected or not
    if (instance_ptr->mqtt_connected_flag == false) {
        return;
    }
    cJSON *user_data = cJSON_CreateObject();

    cJSON_AddNumberToObject(user_data, "sensor_1", 10);
    cJSON_AddNumberToObject(user_data, "sensor_2", 5.5);
    cJSON_AddBoolToObject(user_data, "sensor_3", true);

    iotex_dev_access_data_upload_with_userdata(user_data, 1, IOTEX_USER_DATA_TYPE_JSON);
}

void iotex_devnet_upload_data_example_pb(void)
{
    if (instance_ptr->mqtt_connected_flag == false) {
        return;
    }
    unsigned char sensor_buf[user_data_size] = {0};
    pb_ostream_t  ostream_sensor             = {0};
    user_data     sensor                     = user_data_init_zero;

    sensor.sensor_1                          = 32;
    sensor.sensor_2                          = 64.128;
    sensor.sensor_3                          = true;

    ostream_sensor                           = pb_ostream_from_buffer(sensor_buf, user_data_size);
    if (!pb_encode(&ostream_sensor, user_data_fields, &sensor)) {
        printf("pb encode [event] error in [%s]\n", PB_GET_ERROR(&ostream_sensor));
        return;
    }

    iotex_dev_access_data_upload_with_userdata(sensor_buf, ostream_sensor.bytes_written, IOTEX_USER_DATA_TYPE_PB);
}

/****************** IoTex Application Entry ******************/

int         flag = 1;
extern void default_SetSeed(unsigned int seed);

static void iotex_app_task(void *pvParameters)
{
    // Wait for Wi-Fi to be connected
    while (!get_mqtt_net_flag()) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    time_t    now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2023 - 1900)) {
        ESP_LOGW(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }

    char strftime_buf[64];

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    iotex_wsiotsdk_init(iotex_time_set_func, iotex_mqtt_pubscription, iotex_mqtt_subscription);

    default_SetSeed(esp_random());
    iotex_import_key_example();

    // Start the MQTT
    if (esp_event_post_to(mqtt_app_event_handle, MQTT_APP_EVENT_BASE, MQTT_APP_START, &instance_ptr, sizeof(instance_ptr), portMAX_DELAY) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_post_to error");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    while (1) {

        vTaskDelay(5000 / portTICK_PERIOD_MS);

        iotex_devnet_upload_data_example_json();

        vTaskDelay(5000 / portTICK_PERIOD_MS);
        iotex_devnet_upload_data_example_pb();

        vTaskDelay(5000 / portTICK_PERIOD_MS);
        iotex_devnet_upload_data_example_raw();
    }
}

/****************** MQTT client init ******************/
static void _mqtt_iotex_init(instance_mqtt *instance)
{
    if (instance == NULL) {
        ESP_LOGE(TAG, "instance is NULL");
        return;
    }

    instance->mqtt_name           = TAG;
    instance->mqtt_connected_flag = false;
    instance->mqtt_client         = NULL; // client handler, use for start and stop
    instance->mqtt_cfg            = NULL;
    instance->mqtt_event_handler  = mqtt_event_handler;
    instance->mqtt_starter        = _mqtt_iotex_start;
}

/****************** Public Function Definitions ******************/
void mqtt_iotex_init(void)
{
    mqtt_iotex_instance.is_using = true;
    /*instance_ptr = &mqtt_iotex_instance;*/
    _mqtt_iotex_init(instance_ptr);

    // create a task for Iotex_App
    xTaskCreate(iotex_app_task, "iotex_app_task", 4096 * 2, NULL, 5, NULL);
}
