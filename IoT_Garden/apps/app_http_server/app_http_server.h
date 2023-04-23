#ifndef APP_HTTP_SERVER_H
#define APP_HTTP_SERVER_H

#include <stdint.h>

typedef void (*http_post_callback_function_t)(char *, uint8_t);
// typedef void (*http_get_callback_Function_t)(char *, char *);

void APP_HTTP_SERVER_Start(void);
void APP_HTTP_SERVER_Stop(void);
void APP_HTTP_SERVER_PostSetCallback(http_post_callback_function_t callback_function);
// void APP_HTTP_SERVER_GetSetCallback(http_get_callback_Function_t callback_function);
// void APP_HTTP_SERVER_SendRespone(char *data, uint8_t length);

#endif
