#include "dht.h"

#include "esp_timer.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GET_TICK                (xTaskGetTickCount() * portTICK_PERIOD_MS)

#define DHT11_START_SIGNAL      100000  // in us
#define DHT22_START_SIGNAL      2000    // in us

#define WAITING_FOR_NEXT_READ   2000    // in ms

static uint32_t __WaitingSignal(DHT_DATA_t *dht, uint16_t ms, int level)
{
    uint32_t micros_ticks = 0;
    while(gpio_get_level(dht->pin) == level)
    { 
        if(micros_ticks++ > ms) 
            return DHT_TIMEOUT_ERROR;
        ets_delay_us(1);
    }
    return micros_ticks;
}

static void __SendingStartSignal(DHT_DATA_t *dht)
{
    gpio_set_direction(dht->pin, GPIO_MODE_OUTPUT);
    gpio_set_level(dht->pin, 0);

    switch(dht->type)
    {
        case DHT11:

            break;
        case DHT21:

            break;
        case DHT22:
            ets_delay_us(DHT22_START_SIGNAL);
            break;
        default:
            break;
    }
    
    gpio_set_level(dht->pin, 1);
    ets_delay_us(40);
    gpio_set_direction(dht->pin, GPIO_MODE_INPUT);
}

static int8_t __CheckingRespone(DHT_DATA_t *dht)
{
    /* Wait for next step ~80us*/
    if(__WaitingSignal(dht, 80, 0) == DHT_TIMEOUT_ERROR)
        return DHT_TIMEOUT_ERROR;

    /* Wait for next step ~80us*/
    if(__WaitingSignal(dht, 80, 1) == DHT_TIMEOUT_ERROR) 
        return DHT_TIMEOUT_ERROR;

    return DHT_OK;
}

static int8_t __CheckingCRC(uint8_t data[])
{
    if(data[4] == (data[0] + data[1] + data[2] + data[3]))
        return DHT_OK;
    else
        return DHT_CRC_ERROR;
}

// static struct dht11_reading _timeoutError() {
//     struct dht11_reading timeoutError = {DHT_TIMEOUT_ERROR, -1, -1};
//     return timeoutError;
// }

// static struct dht11_reading _crcError() {
//     struct dht11_reading crcError = {DHT_CRC_ERROR, -1, -1};
//     return crcError;
// }

void DHT_Init(DHT_DATA_t *dht, uint16_t pin, DHT_TYPE_t dht_type)
{
    dht->pin = pin;
    dht->type = dht_type;
    dht->temperature = 0;
    dht->humidity = 0;
}

int8_t DHT_Read(DHT_DATA_t *dht)
{
    /* Tried to sense too son since last read (dht11 needs ~2 seconds to make a new read) */
    if(GET_TICK - dht->last_reading_time < WAITING_FOR_NEXT_READ)
    {
        return DHT_NOT_IN_TIME;
    }

    dht->last_reading_time = GET_TICK;

    uint8_t data[5] = {0,0,0,0,0};

    __SendingStartSignal(dht);

    if(__CheckingRespone(dht) == DHT_TIMEOUT_ERROR)
        return DHT_TIMEOUT_ERROR;
    
    /* Read response */
    for(int i = 0; i < 40; i++) {
        /* Initial data */
        if(__WaitingSignal(dht, 50, 0) == DHT_TIMEOUT_ERROR)
            return DHT_TIMEOUT_ERROR;
                
        if(__WaitingSignal(dht, 70, 1) > 28) {
            /* Bit received was a 1 */
            data[i/8] |= (1 << (7-(i%8)));
        }
    }

    if(__CheckingCRC(data) == DHT_OK) {
        printf("%02x %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3], data[4]);
        
        switch(dht->type)
        {
            case DHT11:

                break;
            case DHT21:

                break;
            case DHT22:
                dht->temperature = (((uint16_t)data[2] << 8) | data[3])/10.0;
                dht->humidity = (((uint16_t)data[0] << 8) | data[2])/10.0;
                break;
        }

        return DHT_OK;
    } else {
        return DHT_CRC_ERROR;
    }
}
