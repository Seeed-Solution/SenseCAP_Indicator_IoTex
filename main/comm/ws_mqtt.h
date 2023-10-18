#ifndef _WS_MQTT_H_
#define _WS_MQTT_H_

#include "model_data.h"
#include "upload_data.h"

#define WS_MQTT_STATUS_INIT                                     0
#define WS_MQTT_STATUS_TOKEN_SERVER_CONNECTING                  1
#define WS_MQTT_STATUS_TOKEN_SERVER_CONNECTED                   2
#define WS_MQTT_STATUS_TOKEN_SERVER_SUBSCRIBED                  3
#define WS_MQTT_STATUS_TOKEN_SERVER_PUBLISHED                   4
#define WS_MQTT_STATUS_TOKEN_SERVER_RECEIVED                    5
#define WS_MQTT_STATUS_TOKEN_SERVER_FINISH                      6
#define WS_MQTT_STATUS_APP_SERVER_START                         7
#define WS_MQTT_STATUS_APP_SERVER_CONNECTED                     8
#define WS_MQTT_STATUS_APP_SERVER_STATUS_SUBSCRIBED             9
#define WS_MQTT_STATUS_APP_SERVER_STATUS                        10
#define WS_MQTT_STATUS_APP_SERVER_RECONNECT                     11
#define WS_MQTT_STATUS_APP_SERVER_BIND_STATUS_CONFIRM           12
#define WS_MQTT_STATUS_APP_SERVER_BIND_STATUS_SUCCESS           13
#define WS_MQTT_STATUS_MAX                                      14

#define WS_MQTT_TOKEN_SERVER_URL                                "mqtt://gateway.w3bstream.com:1883"
#define WS_MQTT_TOKEN_SERVER_TOPIC                              "project"
#define WS_MQTT_TOKEN_SERVER_SUBSCRIPTION_TOPIC                 "project"
#define WS_MQTT_TOKEN_SERVER_PROJECT_NAME                       "seeed-staging"     
//#define WS_MQTT_TOKEN_SERVER_PROJECT_NAME                       "seeed-test"
#define WS_MQTT_TOKEN_SERVER_SUBSCRIPTION_TOPIC                 "project"

#define WS_MQTT_TEST_DEV_SN                                     "114993069231500025"    


#define WS_MQTT_HANDLE_USE_STATUS_MACHINE
//#define WS_MQTT_HANDLE_USE_EVENT

#ifdef WS_MQTT_HANDLE_USE_EVENT
enum {
    MQTT_EVENT_INIT = 0,  

    MQTT_EVENT_TOKEN_SERVER_CONNECT,
    MQTT_EVENT_TOKEN_SERVER_CONNECTED,  
    
    MQTT_EVENT_TOKEN_SERVER_SUBSCRIBED,
    MQTT_EVENT_TOKEN_SERVER_PUBLISHED,

    MQTT_EVENT_TOKEN_SERVER_RECEIVED, 
    MQTT_EVENT_TOKEN_SERVER_FINISH,

    MQTT_EVENT_APP_SERVER_START,
    MQTT_EVENT_APP_SERVER_CONNECTED,

    MQTT_EVENT_ALL,
};
#endif

enum mqtt_server_type {

    MQTT_SERVER_TOKEN = 1,
    MQTT_SERVER_APP,

};

ESP_EVENT_DECLARE_BASE(MQTT_EVENT_BASE);
extern esp_event_loop_handle_t mqtt_event_handle;

int iotex_mqtt_pubscription(unsigned char *topic, unsigned char *buf, unsigned int buflen, int qos);
int iotex_mqtt_subscription(unsigned char *topic);

int ws_mqtt_init(void);


#endif