/* UART Events Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi_station_app.h"
#include "subdriver_gpio.h"

static const char *TAG = "uart_events";

/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM    (1)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart0_queue;

#define GPIO_BUTTON_PIN         9
#define GPIO_RGB_RED_PIN        3
#define GPIO_RGB_GREEN_PIN      4
#define GPIO_RGB_BLUE_PIN       5
#define GPIO_ORANGE_LED         18
#define GPIO_WHITE_LED          19

static EventGroupHandle_t cmd_handle_event_group;
#define LED_WHITE_ON_BIT        (1 << 0)
#define LED_WHITE_OFF_BIT       (1 << 1)
#define LED_ORANGE_ON_BIT       (1 << 2)
#define LED_ORANGE_OFF_BIT      (1 << 3)
#define WIFI_CONNECTING_BIT     (1 << 4)

char ssid[32], pass[32];

void cmd_handle_task()
{
    while(1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(cmd_handle_event_group, 0x0FFF, pdTRUE, pdFALSE, portMAX_DELAY);
        if(uxBits & WIFI_CONNECTING_BIT)
        {
            printf("SSID: %s\nPASS: %s\n", ssid, pass);
            WIFI_STA_APP_Connect(ssid, pass);
        }
        if(uxBits & LED_WHITE_ON_BIT)
        {
            SUBDRIVER_GPIO_SetState(GPIO_WHITE_LED, 1);
        }
        if(uxBits & LED_WHITE_OFF_BIT)
        {
            SUBDRIVER_GPIO_SetState(GPIO_WHITE_LED, 0);
        }
        if(uxBits & LED_ORANGE_ON_BIT)
        {
            SUBDRIVER_GPIO_SetState(GPIO_ORANGE_LED, 1);
        }
        if(uxBits & LED_ORANGE_OFF_BIT)
        {
            SUBDRIVER_GPIO_SetState(GPIO_ORANGE_LED, 0);
        }
    }
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    // ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    // uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    // ESP_LOGI(TAG, "[DATA EVT]:");
                    // uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                    int pos = uart_pattern_pop_pos(EX_UART_NUM);
                    ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                    if (pos == -1) {
                        // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                        // record the position. We should set a larger queue size.
                        // As an example, we directly flush the rx buffer here.
                        uart_flush_input(EX_UART_NUM);
                    } else {
                        uart_read_bytes(EX_UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                        char *arg[10];
                        uint8_t num_arg = 0;
                        char *token = strtok((char *)dtmp, " ");
                        arg[num_arg++] = token;

                        if(strstr(arg[0], "SSID"))
                        {
                            strcpy(ssid, (char *)(dtmp + 5));
                            printf("%s\n%s\nOK\n", (char *)dtmp, (char *)(dtmp + 5));
                            // continue;
                        }

                        if(strstr(arg[0], "PASS"))
                        {
                            strcpy(pass, (char *)(dtmp + 5));
                            printf("%s\n%d\nOK\n", (char *)dtmp, *(char *)(dtmp + 5));
                            // continue;
                        }

                        while(token != NULL)
                        {
                            token = strtok(NULL, " ");
                            arg[num_arg++] = token;
                        }
                        
                        if(strstr(arg[0], "WIFI_CONNECT"))
                        {
                            printf("OK\n");
                            xEventGroupSetBits(cmd_handle_event_group, WIFI_CONNECTING_BIT);
                        }
                        else if(strstr(arg[0], "LED_WHITE"))
                        {
                            if(strstr(arg[1], "ON"))
                            {
                                xEventGroupSetBits(cmd_handle_event_group, LED_WHITE_ON_BIT);
                            }
                            else if(strstr(arg[1], "OFF"))
                            {
                                xEventGroupSetBits(cmd_handle_event_group, LED_WHITE_OFF_BIT);
                            }
                        }
                        else if(strstr(arg[0], "LED_ORANGE"))
                        {
                            if(strstr(arg[1], "ON"))
                            {
                                xEventGroupSetBits(cmd_handle_event_group, LED_ORANGE_ON_BIT);
                            }
                            else if(strstr(arg[1], "OFF"))
                            {
                                xEventGroupSetBits(cmd_handle_event_group, LED_ORANGE_OFF_BIT);
                            }
                        }
                    }
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    //Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);

    //Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '\n', PATTERN_CHR_NUM, 9, 0, 0);
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(EX_UART_NUM, 20);  

    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 12, NULL);

    SUBDRIVER_GPIO_OutputInit(GPIO_ORANGE_LED);
    SUBDRIVER_GPIO_OutputInit(GPIO_WHITE_LED);
    cmd_handle_event_group = xEventGroupCreate();
    xTaskCreate(cmd_handle_task, "cmd_handle_task", 4096, NULL, 13, NULL);

}
