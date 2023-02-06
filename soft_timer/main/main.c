#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

TimerHandle_t timer[2];

void TIMER_Elapsed_Callback(TimerHandle_t htim)
{
    configASSERT(htim);
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(htim);

    switch(timer_id)
    {
        case 0:
            printf("Timer 0 callback every 500ms\n");
            break;
        case 1:
            printf("Timer 1 callback every 1000ms\n");
            break;
    }
}

void TASK_Blink_Function(void *param)
{
    uint8_t led_state = 0;
    while(1)
    {
        printf("Led's state is %s\n", (led_state == 1) ? "ON" : "OFF");
        led_state = !led_state;
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    timer[0] = xTimerCreate("Blink_Timer", 500 / portTICK_PERIOD_MS, pdTRUE, (void *)0, (void *)TIMER_Elapsed_Callback);
    timer[1] = xTimerCreate("Blink_Timer", 1000 / portTICK_PERIOD_MS, pdTRUE, (void *)1, (void *)TIMER_Elapsed_Callback);

    xTimerStart(timer[0], 0);
    xTimerStart(timer[1], 0);

    xTaskCreate(TASK_Blink_Function, "Blink_Task", 2048, NULL, 15, NULL);

}
