#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>
#include <esp_http_server.h>

typedef void (*switch_callback_function_t)(int state);
typedef void (*slider_callback_function_t)(int duty);
typedef void (*wifi_info_callback_function_t)(char *ssid, char *pass);

void HTTP_SERVER_Start(void);
void HTTP_SERVER_Stop(void);
void switch_set_callback(void (*callback_function));
void slider_set_callback(void (*callback_function));
void wifi_send_set_callback(void (*callback_function));

#endif
