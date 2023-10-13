#include "bsp_board.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "indicator_controller.h"
#include "indicator_model.h"
#include "indicator_view.h"

static const char *TAG = "app_main";

#define VERSION  "v1.0.0"

#define SENSECAP "\n\
   _____                      _________    ____         \n\
  / ___/___  ____  ________  / ____/   |  / __ \\       \n\
  \\__ \\/ _ \\/ __ \\/ ___/ _ \\/ /   / /| | / /_/ /   \n\
 ___/ /  __/ / / (__  )  __/ /___/ ___ |/ ____/         \n\
/____/\\___/_/ /_/____/\\___/\\____/_/  |_/_/           \n\
--------------------------------------------------------\n\
 Version: %s %s %s\n\
--------------------------------------------------------\n\
"


void app_main(void)
{
    ESP_LOGI("", SENSECAP, VERSION, __DATE__, __TIME__);

    ESP_ERROR_CHECK(bsp_board_init());
    lv_port_init();

    lv_port_sem_take();
    indicator_view_init();
    lv_port_sem_give();

    indicator_model_init();
    indicator_controller_init();
    w3b_cfg_init();

    // default_SetSeed(esp_random());
    // iotex_wsiotsdk_init(time, iotex_mqtt_pubscription, iotex_mqtt_subscription);

    // ws_mqtt_init();
    // iotex_upload_data_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
