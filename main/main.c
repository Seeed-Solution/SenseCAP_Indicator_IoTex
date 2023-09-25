#include "bsp_board.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_event_base.h"

#include "indicator_model.h"
#include "indicator_view.h"
// #include "indicator_controller.h"

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

ESP_EVENT_DEFINE_BASE(VIEW_EVENT_BASE);
esp_event_loop_handle_t view_event_handle;

ESP_EVENT_DEFINE_BASE(CFG_EVENT_BASE);
esp_event_loop_handle_t cfg_event_handle;

ESP_EVENT_DEFINE_BASE(MQTT_APP_EVENT_BASE);
esp_event_loop_handle_t mqtt_app_event_handle;

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
        .task_core_id    = tskNO_AFFINITY};

    esp_event_loop_args_t cfg_event_task_args = {
        .queue_size      = 5,
        .task_name       = "cfg_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 1024 * 5,
        .task_core_id    = tskNO_AFFINITY};

    esp_event_loop_args_t mqtt_event_task_args = {
        .queue_size      = 3,
        .task_name       = "mqtt_app_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 4096,
        .task_core_id    = tskNO_AFFINITY};

    ESP_ERROR_CHECK(esp_event_loop_create(&view_event_task_args, &view_event_handle));
    ESP_ERROR_CHECK(esp_event_loop_create(&cfg_event_task_args, &cfg_event_handle));
    ESP_ERROR_CHECK(esp_event_loop_create(&mqtt_event_task_args, &mqtt_app_event_handle));

    lv_port_sem_take();
    indicator_view_init();
    lv_port_sem_give();

    indicator_model_init();

    w3b_cfg_init();
    // indicator_controller_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
