#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht.h"

void app_main(void)
{
    DHT_DATA_t dht;
    DHT_Init(&dht, 1, DHT22);
    while(1)
    {
        if(DHT_Read(&dht) == DHT_OK)
        {
            printf("Temp: %2.2f - Humi: %2.2f\n", dht.temperature, dht.humidity);
        }
        else
        {
            printf("DHT11 error.\n");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
