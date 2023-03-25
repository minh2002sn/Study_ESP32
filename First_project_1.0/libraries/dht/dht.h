#ifndef DHT_H
#define DHT_H

#include <stdint.h>

enum{
	DHT11,
	DHT22,
};

enum{
	DHT_FAIL,
	DHT_OK,
};

typedef struct{
	uint8_t sensor_type;
	uint32_t Pin;
	float temperature;
	float humidity;
} DHT_DATA_t;

void DHT_Init(DHT_DATA_t* DHT, uint8_t sensor_type, uint32_t Pin);
uint8_t DHT_Read_Value(DHT_DATA_t* DHT);

#endif