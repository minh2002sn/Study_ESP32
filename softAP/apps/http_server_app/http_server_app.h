#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>
#include <esp_http_server.h>

typedef void (*switch_callback_function_t)(int state);
typedef void (*slider_callback_function_t)(int duty);
typedef void (*wifi_info_callback_function_t)(char *buf, uint8_t buf_len);

// typedef struct
// {
//     httpd_uri http_uri;
//     uint8_t id;
// } HTTP_REQUEST_DATA_t;

// void HTTP_SERVER_RegisterRequest(char *uri, httpd_method_t method, esp_err_t (*callback_function)(httpd_req_t *r));

void HTTP_SERVER_Start(void);
void HTTP_SERVER_Stop(void);
void switch_set_callback(void (*callback_function));
void slider_set_callback(void (*callback_function));
void wifi_send_set_callback(void (*callback_function));

#endif
