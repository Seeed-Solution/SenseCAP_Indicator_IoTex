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
    indicator_view_init(); // LVGL 界面，初始化前端页面相关的部分，以及 event_handler: view_event_handle 在 `view_data.h` 中被定义 event collection
    lv_port_sem_give();

    indicator_model_init(); // 数据后端类型相关，提供数据和配置 handle: model_event_handle & cfg_event_handle 在 `model_data.h` 中被定义 event collection
    indicator_controller_init(); // 时间和显示相关的配置信息
    w3b_cfg_init(); // cmd 提供接口, 实现SN, Wallet, bind flag等的获取 以及对上位机的响应格式;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
