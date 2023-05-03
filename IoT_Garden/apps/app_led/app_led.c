#include <stdio.h>
#include <stdint.h>
#include "app_led.h"
#include "subdriver_gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GET_TICK                (xTaskGetTickCount() * portTICK_PERIOD_MS)

#define TAG                     "APP_LED"

#define LED_PIN                 0
#define LONG_BLINK_CYCLE        1000
#define SHORT_BLINK_CYCLE       200      

static LED_STATE_t led_state = LED_OFF_STATE;
static uint64_t timer = 0;

static TaskHandle_t led_status_task;

static void led_status_task_handle()
{
    SUBDRIVER_GPIO_OutputInit(LED_PIN);
    while(1)
    {
        switch(led_state)
        {
            case LED_OFF_STATE:
                SUBDRIVER_GPIO_SetState(LED_PIN, 0);
                break;
            case LED_ON_STATE:
                SUBDRIVER_GPIO_SetState(LED_PIN, 1);
                break;
            case LED_BLINK_LOW:
                if(GET_TICK - timer > LONG_BLINK_CYCLE/2)
                {
                    SUBDRIVER_GPIO_SetState(LED_PIN, !SUBDRIVER_GPIO_GetState(LED_PIN));
                    timer = GET_TICK;
                }
                break;
            case LED_BLINK_FAST:
                if(GET_TICK - timer > SHORT_BLINK_CYCLE/2)
                {
                    SUBDRIVER_GPIO_SetState(LED_PIN, !SUBDRIVER_GPIO_GetState(LED_PIN));
                    timer = GET_TICK;
                }
                break;
            default:
                break;
        }
    }
}

void APP_LED_Init()
{
    xTaskCreate(led_status_task_handle, "led_status_task_handle", 2048, NULL, tskIDLE_PRIORITY, &led_status_task);
}

void APP_LED_SetState(LED_STATE_t state)
{
    led_state = state;
}
