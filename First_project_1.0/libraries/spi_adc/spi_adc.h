#ifndef SPI_ADC
#define SPI_ADC

#include <stdint.h>
#include "esp_err.h"

enum
{
    SPI_ADC_CH0,
    SPI_ADC_CH1,
    SPI_ADC_CH2,
    SPI_ADC_CH3,
};

typedef struct
{
    uint8_t adc_channel;
    uint16_t raw_value;
} SPI_ADC_DATA_t;

void SPI_ADC_Init(SPI_ADC_DATA_t *spi_adc, uint8_t adc_channel);
esp_err_t SPI_ADC_ReadValue(SPI_ADC_DATA_t *spi_adc);

#endif
