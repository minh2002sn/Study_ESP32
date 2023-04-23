#ifndef APP_NVS_H
#define APP_NVS_H

#include <stdint.h>

void APP_NVS_Init();
void APP_NVS_SetValueU8(char *key, uint8_t value);
void APP_NVS_GetValueU8(char *key, uint8_t *value);
void APP_NVS_SetValueU16(char *key, uint16_t value);
void APP_NVS_GetValueU16(char *key, uint16_t *value);
void APP_NVS_SetValueU32(char *key, uint32_t value);
void APP_NVS_GetValueU32(char *key, uint32_t *value);
void APP_NVS_SetValueU64(char *key, uint64_t value);
void APP_NVS_GetValueU64(char *key, uint64_t *value);
void APP_NVS_SetValueStr(char *key, char *value);
void APP_NVS_GetValueStr(char *key, char *value);

#endif
