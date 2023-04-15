#ifndef BH1750FVI_H
#define BH1750FVI_H

#include <stdint.h>

typedef struct
{
    uint32_t i2c_sda_pin;
    uint32_t i2c_scl_pin;
    uint8_t i2c_num;
    uint8_t address;
    uint16_t illu_value;
} BH1750_DATA_t;

void BH1750_Init(BH1750_DATA_t *bh1750, uint32_t i2c_sda_pin, uint32_t i2c_scl_pin, uint8_t i2c_num, uint8_t addr_pin_state);
void BH1750_OneShotRead(BH1750_DATA_t *bh1750);

#endif
