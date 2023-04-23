#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>

enum dht11_status
{
    DHT_CRC_ERROR = -2,
    DHT_TIMEOUT_ERROR,
    DHT_NOT_IN_TIME,
    DHT_OK
};

typedef enum
{
    DHT11,
    DHT21,
    DHT22,
} DHT_TYPE_t;

typedef struct
{
    uint16_t pin;
    DHT_TYPE_t type;
    uint32_t last_reading_time;
    float temperature;
    float humidity;
} DHT_DATA_t;

void DHT_Init(DHT_DATA_t *dht, uint16_t pin, DHT_TYPE_t dht_type);
int8_t DHT_Read(DHT_DATA_t *dht);

#endif