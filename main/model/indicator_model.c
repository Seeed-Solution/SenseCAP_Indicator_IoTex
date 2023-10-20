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
