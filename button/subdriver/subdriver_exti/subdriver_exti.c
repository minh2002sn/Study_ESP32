#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "subdriver_exti.h"
#include "driver/gpio.h"
#include "esp_attr.h"

static uint8_t                  is_initialized = 0;
static QueueHandle_t            exti_event_queue = NULL;
static TaskHandle_t             exti_handle_task = NULL;
static EXTI_CALLBACK_FUNCTION_t EXTI_CallbackFunction = NULL;

static void IRAM_ATTR GPIO_ISR_Handler(void* arg)
{
    uint32_t gpio_pin = (uint32_t)arg;
    xQueueSendFromISR(exti_event_queue, &gpio_pin, NULL);
}

static void EXTI_Task_Handler()
{
    uint32_t gpio_pin = 0;;
    while(1)
    {
        if(xQueueReceive(exti_event_queue, &gpio_pin, portMAX_DELAY))
        {
            if(EXTI_CallbackFunction != NULL)
                EXTI_CallbackFunction(gpio_pin);
        }
    }
}

void SUBDRIVER_EXTI_Init(uint32_t pin, EXTI_EDGE_t edge)
{
    if(is_initialized == 0)
    {
        exti_event_queue = xQueueCreate(10, sizeof(uint32_t));
        xTaskCreate(EXTI_Task_Handler, "exti_handle_task", 2048, NULL, 10, &exti_handle_task);
        is_initialized = 1;
    }

    gpio_config_t io_conf = 
    {
        .pin_bit_mask   = (1ULL << pin),
        .mode           = GPIO_MODE_INPUT,
        .pull_up_en     = GPIO_PULLUP_ENABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = edge,
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, GPIO_ISR_Handler, (void*) pin);
}

void SUBDRIVER_EXTI_SetCallbackFunction(EXTI_CALLBACK_FUNCTION_t CallbackFunction)
{
    EXTI_CallbackFunction = CallbackFunction;
}

