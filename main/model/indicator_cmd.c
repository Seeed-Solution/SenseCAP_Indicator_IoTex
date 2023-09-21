#include "indicator_cmd.h"
#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include "indicator_storage.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char *TAG = "CMD_RESP";

#define PROMPT_STR "Indicator"

static int fn_w3b_cm_read(int argc, char **argv)
{
    if (esp_event_post_to(cfg_event_handle, CFG_EVENT_BASE, CFG_EVENT_READ, NULL, 0, portMAX_DELAY) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_post_to error");
        return -1;
    }
    return 0;
}

static void register_w3b_cmd_read(void)
{

    const esp_console_cmd_t cmd = {
        .command = "Read_Device_Info",
        .help    = NULL,
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
    if (w3b_cmd_args.sn->count == 0 || w3b_cmd_args.wallet_addr->count == 0) {
        ESP_LOGE(TAG, "w3b_cm_set: sn or wallet_addr is null");
        response_cmd(CFG_FAIL, "sn or wallet_addr is null");
        return -1;
    }

    int sn_len  = strlen(w3b_cmd_args.sn->sval[0]);
    int wad_len = strlen(w3b_cmd_args.wallet_addr->sval[0]);
    if (sn_len > SN_MAX_LEN) {
        ESP_LOGE(TAG, "w3b_cm_set: sn %d is too long", sn_len);
        response_cmd(CFG_FAIL, "sn is too long");
        return -1;
    }
    if (wad_len > WALLET_MAX_LEN) {
        ESP_LOGE(TAG, "w3b_cm_set: wallet_addr %d is too long", wad_len);
        response_cmd(CFG_FAIL, "wallet_addr is too long");
        return -1;
    }
    w3b_cfg_interface cfg;
    memset(&cfg, 0, sizeof(w3b_cfg_interface));
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
        .help     = NULL,
        .hint     = NULL,
        .func     = &fn_w3b_cm_set,
        .argtable = &w3b_cmd_args,
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

    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}