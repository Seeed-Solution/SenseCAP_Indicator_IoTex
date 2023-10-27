#include "bsp_board.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "indicator_controller.h"
#include "indicator_model.h"
#include "indicator_view.h"

#include "wsiotsdk.h"
#include "ws_mqtt.h"
#include "upload_data.h"

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

    esp_event_loop_args_t view_event_task_args = {
        .queue_size      = 10,
        .task_name       = "view_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 10240,
        .task_core_id    = tskNO_AFFINITY,
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&view_event_task_args, &view_event_handle));

    esp_event_loop_args_t model_event_task_args = {
        .queue_size      = 10,
        .task_name       = "model_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 1024 * 20,
        .task_core_id    = tskNO_AFFINITY};
    ESP_ERROR_CHECK(esp_event_loop_create(&model_event_task_args, &model_event_handle));

    esp_event_loop_args_t cfg_event_task_args = {
        .queue_size      = 5,
        .task_name       = "cfg_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 1024 * 5,
        .task_core_id    = tskNO_AFFINITY,
    };
    ESP_ERROR_CHECK(esp_event_loop_create(&cfg_event_task_args, &cfg_event_handle));


    lv_port_sem_take();
    indicator_view_init(); // LVGL 界面，初始化前端页面相关的部分，以及 event_handler: view_event_handle 在 `view_data.h` 中被定义 event collection
    lv_port_sem_give();

    indicator_model_init();      // 数据后端类型相关，提供数据和配置 handle: model_event_handle & cfg_event_handle 在 `model_data.h` 中被定义 event collection
    indicator_controller_init(); // 时间和显示相关的配置信息
    w3b_cfg_init(); // cmd 提供接口, 实现SN, Wallet, bind flag等的获取 以及对上位机的响应格式;

    default_SetSeed(esp_random());
    iotex_wsiotsdk_init(time, iotex_mqtt_pubscription, iotex_mqtt_subscription);

    iotex_ws_comm_init();
    iotex_upload_data_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
