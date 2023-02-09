#include <stdint.h>
#include <inttypes.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define PRESSING_TIMEOUT    2000 // ms
#define GET_TICK            (xTaskGetTickCountFromISR() * portTICK_PERIOD_MS)
#define CHECK(value)        printf("%d\n", value == ESP_OK) 

#define BUTTON_PRESSING_BIT         (1 << 0)
#define BUTTON_RELEASING_BIT        (1 << 1)
#define BUTTON_SHORT_PRESSING_BIT   (1 << 2)
#define BUTTON_LONG_PRESSING_BIT    (1 << 3)

static QueueHandle_t        button_event_queue = NULL;
static TimerHandle_t        button_soft_timer = NULL;
static TaskHandle_t         button_debouncing_task = NULL;
static TaskHandle_t         button_executing_task = NULL;
static EventGroupHandle_t   button_executing_event_group = NULL;
static uint8_t              is_defined = 0;
static ButtonHandle_t       *current_button = NULL;

static void IRAM_ATTR GPIO_ISR_Handler(void* arg)
{
    xQueueSendFromISR(button_event_queue, (uint32_t *)&arg, NULL);
}

static void button_debouncing_task_handler(void* arg)
{
    uint32_t io_pin_num;
    while(1)
    {
        if(xQueueReceive(button_event_queue, &io_pin_num, portMAX_DELAY))
        {
            // printf("GPIO[%"PRIu32"] intr, val: %d\n", io_pin_num, gpio_get_level(io_pin_num));
            if(io_pin_num == gpio_pin)
            {
                if(GET_TICK - deboucing_timer > 20000)
                {
                    continue;
                }
                deboucing_timer = GET_TICK;

                if(gpio_get_level(gpio_pin) == 0)
                {
                    is_pressing = 1;
                    pressing_timer = GET_TICK;
                    if(BUTTON_PressingCallback != NULL)
                        BUTTON_PressingCallback(gpio_pin);
                }
                else
                {
                    if(BUTTON_ReleasingCallback != NULL)
                        BUTTON_ReleasingCallback(gpio_pin);
                    if(GET_TICK - pressing_timer < PRESSING_TIMEOUT)
                    {

                    }
                }
            }
        }
    }
}

static void soft_timer_callback()
{

}

static void button_executing_task_handler(void* arg)
{
    while(1)
    {
        
    }
}

void BUTTON_Init(int pin)
{
    if(is_defined == 0)
    {
        button_event_queue = xQueueCreate(10, sizeof(uint32_t));
        button_soft_timer = xTimerCreate("button_soft_timer", PRESSING_TIMEOUT / portTICK_PERIOD_MS, pdFALSE, (void *)0, soft_timer_callback);
        button_debouncing_task = xTaskCreate(button_debouncing_task_handler, "button_debouncing_task", 2048, NULL, 10, NULL);
        button_executing_task = xTaskCreate(button_executing_task_handler, "button_executing_task_", 1024, NULL, 10, NULL);
        button_executing_event_group = xEventGroupCreate();
        is_defined = 1;
    }
    gpio_pin = pin;

    gpio_config_t io_conf = 
    {
        .pin_bit_mask   = (1ULL << pin),
        .mode           = GPIO_MODE_INPUT,
        .pull_up_en     = GPIO_PULLUP_ENABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = GPIO_INTR_ANYEDGE,
    };

    gpio_config(&io_conf);    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, GPIO_ISR_Handler, (void*) pin);    
}

void BUTTON_Set_Callback_Function(void *PressingCallback, void *ReleasingCallback)
{
    BUTTON_PressingCallback = PressingCallback;
    BUTTON_ReleasingCallback = ReleasingCallback;
}
