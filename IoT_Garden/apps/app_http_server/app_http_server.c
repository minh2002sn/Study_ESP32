#include "app_http_server.h"

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include <esp_http_server.h>
#include <stdio.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */
static const char *TAG = "example";

httpd_handle_t server = NULL;

extern const unsigned char index_wifi_html_start[] asm("_binary_setting_wifi_web_html_start");
extern const unsigned char index_wifi_html_end[] asm("_binary_setting_wifi_web_html_end");

static http_post_callback_function_t post_callback_function = NULL;
static uint8_t is_http_server_started = 0;

/* An HTTP GET handler */
static esp_err_t setting_wifi_web_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_wifi_html_start, index_wifi_html_end - index_wifi_html_start);

    return ESP_OK;
}

static const httpd_uri_t setting_wifi_web = {
    .uri       = "/setting_wifi",
    .method    = HTTP_GET,
    .handler   = setting_wifi_web_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

/* An HTTP POST handler */
static esp_err_t send_wifi_info_button_post_handler(httpd_req_t *req)
{
    char buf[100];

    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    // char *ssid, *pass;
    // if(buf != NULL)
    // {
    //     ssid = strtok(buf, "\n");
    //     pass = strtok(NULL, "\n");
    // }
    // else
    // {
    //     return ESP_FAIL;
    // }

    if(post_callback_function != NULL)
        post_callback_function(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t send_wifi_info_button = {
    .uri       = "/wifi_info",
    .method    = HTTP_POST,
    .handler   = send_wifi_info_button_post_handler,
    .user_ctx  = NULL
};

void APP_HTTP_SERVER_Start(void)
{
    if(is_http_server_started) return;
    is_http_server_started = 1;
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &send_wifi_info_button);
        httpd_register_uri_handler(server, &setting_wifi_web);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void APP_HTTP_SERVER_Stop(void)
{
    // Stop the httpd server
    httpd_stop(server);
    is_http_server_started = 0;
}

void APP_HTTP_SERVER_PostSetCallback(http_post_callback_function_t callback_function)
{
    post_callback_function = callback_function;
}
