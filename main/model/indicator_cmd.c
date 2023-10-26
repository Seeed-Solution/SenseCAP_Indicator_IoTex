/**
 * @file indicator_cmd.c
 * @brief
 * 处理来自上位机的数据
 */
#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "esp_mac.h"
#include "indicator_model.h"
#include "indicator_storage.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#ifndef COMPACT_MACSTR
#define COMPACT_MACSTR "%02x%02x%02x%02x%02x%02x"
#endif
static const char *TAG = "CMD_RESP";

#define PROMPT_STR "Indicator"

eth_cfg eth_address = {0};

static int fn_w3b_cm_read(int argc, char **argv)
{
    // bool cmd_flag = true;
    // if (esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_READ, &cmd_flag, sizeof(bool), portMAX_DELAY) != ESP_OK) {
    //     ESP_LOGE(TAG, "esp_event_post_to error");
    //     return -1;
    // }
    W3B_CFG w3b_cfg;
    int     len = sizeof(W3B_CFG);
    if (cfg_read_fn(&w3b_cfg, &len) != ESP_OK) {
        ESP_LOGE(TAG, "cfg_read_fn failed");
        response_cmd(CFG_FAIL, "Read failed");
        return -1;
    }
    char buf[526];
    // sprintf(buf, "CMD_RESP:%s," MACSTR ",%s,%s\r\n", "OK", MAC2STR(w3b_cfg.mac), w3b_cfg.cfg.sn, w3b_cfg.cfg.wallet);
    sprintf(buf, "CMD_RESP:%s," COMPACT_MACSTR ",%s,%s\r\n", "OK", MAC2STR(w3b_cfg.mac), w3b_cfg.cfg.sn, eth_address.eth);
    printf("%s", buf);
    return 0;
}

static void register_w3b_cmd_read(void)
{

    const esp_console_cmd_t cmd = {
        .command = "Read_Device_Info",
        .help    = "Read Device Info",
        .hint    = NULL,
        .func    = &fn_w3b_cm_read,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static struct {
    struct arg_str *sn;
    struct arg_str *wallet_addr;
    struct arg_end *end;
} w3b_cmd_args;

static int fn_w3b_cm_set(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&w3b_cmd_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, w3b_cmd_args.end, argv[0]);
        return 1;
    }

    w3b_cfg_interface cfg;
    int               cfg_len;
    memset(&cfg, 0, sizeof(w3b_cfg_interface));

    int wad_len = 0;
    int sn_len  = 0;
    if (!w3b_cmd_args.wallet_addr->count && !w3b_cmd_args.sn->count) {
        ESP_LOGE(TAG, "w3b_cm_set: sn and wallet_addr is null");
        response_cmd(CFG_FAIL, "sn and wallet_addr is null");
        return -1;
    }

    if (w3b_cmd_args.wallet_addr->count > 0) {
        wad_len = strlen(w3b_cmd_args.wallet_addr->sval[0]);
        ESP_LOGI(TAG, "w3b_cm_set: wallet_addr is %s, len:%d", w3b_cmd_args.wallet_addr->sval[0], wad_len);
        if (wad_len > WALLET_MAX_LEN) {
            ESP_LOGE(TAG, "w3b_cm_set: wallet_addr %d is too long", wad_len);
            // response_cmd(CFG_FAIL, "wallet_addr is too long");
            // return -1;
        }
    } else {
        ESP_LOGW(TAG, "w3b_cm_set: wallet_addr is null");
    }

    if (w3b_cmd_args.sn->count > 0) {
        sn_len = strlen(w3b_cmd_args.sn->sval[0]);
        ESP_LOGI(TAG, "w3b_cm_set: sn is %s, len:%d", w3b_cmd_args.sn->sval[0], sn_len);
        if (sn_len > SN_MAX_LEN) {
            ESP_LOGE(TAG, "w3b_cm_set: sn %d is too long", sn_len);
            response_cmd(CFG_FAIL, "sn is too long");
            return -1;
        }
    } else {
        ESP_LOGW(TAG, "w3b_cm_set: sn is null");
        // response_cmd(CFG_FAIL, "sn is null");
        // return -1;
    }

    cfg.sn_len     = sn_len;
    cfg.wallet_len = wad_len;
    memcpy(cfg.sn, w3b_cmd_args.sn->sval[0], sn_len);
    memcpy(cfg.wallet, w3b_cmd_args.wallet_addr->sval[0], wad_len);

    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_WRITE, &cfg, sizeof(w3b_cfg_interface), portMAX_DELAY);
    return ESP_OK;
}

static void register_w3b_cmd_set(void)
{
    w3b_cmd_args.sn             = arg_str0("s", "sn", "<s>", "set S/N");
    w3b_cmd_args.wallet_addr    = arg_str0("w", "wallet", "<wallet>", "set wallet address");
    w3b_cmd_args.end            = arg_end(2);

    const esp_console_cmd_t cmd = {
        .command  = "Device_PIN",
        .help     = "Set Device Info",
        .hint     = NULL,
        .func     = &fn_w3b_cm_set,
        .argtable = &w3b_cmd_args,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int fn_clean_bind(int argc, char **argv)
{
    bool bind_flag = false;
    esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, BIND_EVENT_WRITE, &bind_flag, sizeof(bool), portMAX_DELAY);
    return 0;
}

static void register_clean_bind(void)
{

    const esp_console_cmd_t cmd = {
        .command = "clear",
        .help    = "clear bind flag",
        .hint    = NULL,
        .func    = &fn_clean_bind,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}
static struct {
    struct arg_int *server_status;
    struct arg_end *end;
} server_test_args;

static int fn_server_test_cmd(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&server_test_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, server_test_args.end, argv[0]);
        return 1;
    }
    switch (server_test_args.server_status->ival[0]) {
        case IOTEX_STATUS_NO_RESPONSE:
            ESP_LOGI(TAG, "event: IOTEX_STATUS_NO_RESPONSE");
            esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_NO_RESPONSE, NULL, NULL, portMAX_DELAY);
            break;
        case IOTEX_STATUS_DEVICE_SHOULD_ENROLL: { // 设备没有在 Potal 页面上注册，需要注册，弹窗提示
            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_SHOULD_ENROLL");
            esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SHOULD_ENROLL, NULL, NULL, portMAX_DELAY);
            break;
        }
        case IOTEX_STATUS_DEVICE_CONFIRM_NEEDED: { // 设备（已存在的SN）已经在 Potal 页面上注册，但没有确认
            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_CONFIRM_NEEDED");
            esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_CONFIRM_NEEDED, NULL, NULL, portMAX_DELAY);
            break;
        }
        case IOTEX_STATUS_DEVICE_SUCCESS: { // 设备已经绑定成功
            ESP_LOGI(TAG, "event: IOTEX_STATUS_DEVICE_SUCCESS");
            esp_event_post_to(model_event_handle, DATA_EVENT_BASE, IOTEX_STATUS_DEVICE_SUCCESS, NULL, NULL, portMAX_DELAY);

            break;
        }
        default:
            break;
    }
    return ESP_OK;
}

static void register_server_test_cmd(void)
{
    server_test_args.server_status = arg_int0("s", "status", "<int>", "1,2,3,4");
    server_test_args.end           = arg_end(1); // 限制选项的数量为 1

    const esp_console_cmd_t cmd    = {
           .command  = "test",
           .help     = "server test",
           .hint     = NULL,
           .func     = &fn_server_test_cmd,
           .argtable = &server_test_args,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

int indicator_cmd_init(void)
{
    esp_console_repl_t       *repl        = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    repl_config.prompt                    = PROMPT_STR ">";
    repl_config.max_cmdline_length        = 1024;

    register_w3b_cmd_set();
    register_w3b_cmd_read();
    register_clean_bind();
    register_server_test_cmd();

    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}

#define EnumToStr(x) #x
void response_cmd(CFG_STATUS status, char *resp)
{
    static const char *OK   = "OK";
    static const char *FAIL = "FAIL";
    switch (status) {
        case CFG_OK:
            printf("CMD_RESP:%s\r\n", OK);
            break;
        case CFG_FAIL:
            if (resp == NULL)
                resp = "\0";
            printf("CMD_RESP:%s,%s\r\n", FAIL, resp);
            break;
        default:
            break;
    }
}
#undef EnumToStr