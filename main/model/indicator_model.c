#include "indicator_model.h"
#include "indicator_cmd.h"
#include "indicator_display.h"
#include "indicator_storage.h"

ESP_EVENT_DEFINE_BASE(DATA_EVENT_BASE);
ESP_EVENT_DEFINE_BASE(MODEL_EVENT_BASE);
esp_event_loop_handle_t model_event_handle;

ESP_EVENT_DEFINE_BASE(CFG_EVENT_BASE); // Config
esp_event_loop_handle_t cfg_event_handle;

extern int indicator_city_init(void);
extern int indicator_sensor_init(void);
extern int indicator_time_init(void);
extern int indicator_wifi_init(void);
extern int indicator_cmd_init(void);
extern int indicator_btn_init(void);

int indicator_model_init(void)
{
    esp_event_loop_args_t model_event_task_args = {
        .queue_size      = 3,
        .task_name       = "model_event_task",
        .task_priority   = uxTaskPriorityGet(NULL),
        .task_stack_size = 4096,
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


    indicator_storage_init();
    indicator_btn_init();
    indicator_sensor_init(); // Get Sensor Data
    indicator_wifi_init();
    indicator_time_init();
    indicator_city_init();
    indicator_display_init(); // lcd bl on
    indicator_cmd_init();
    indicator_iotex_init();
}
