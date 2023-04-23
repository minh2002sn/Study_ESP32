#ifndef APP_MQTT_H
#define APP_MQTT_H

#include <stdint.h>

void APP_MQTT_Start(void);
void APP_MQTT_SendJson(char *topic, char *json_data);

#endif
