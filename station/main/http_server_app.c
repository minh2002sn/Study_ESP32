#include "http_server_app.h"
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

extern const unsigned char index_html_start[] asm("_binary_web_html_start");
extern const unsigned char index_html_end[] asm("_binary_web_html_end");

/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = "Hello cc";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

/* An HTTP GET handler */
static esp_err_t web_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);

    return ESP_OK;
}

static const httpd_uri_t web = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = web_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

/* An HTTP GET handler */
static esp_err_t dht11_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    char respone_str[100]= "";
    float temp = 27.25;
    float humi = 80.91;
    sprintf(respone_str, "{\"TEMPERATURE\":\"%0.2f\",\"HUMIDITY\":\"%0.2f\"}", temp, humi);
    // httpd_resp_set_type(req, "json/html");
    httpd_resp_send(req, respone_str, strlen(respone_str));

    return ESP_OK;
}

static const httpd_uri_t dht11_get = {
    .uri       = "/getdht11",
    .method    = HTTP_GET,
    .handler   = dht11_get_handler,
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
static esp_err_t data_post_handler(httpd_req_t *req)
{
    char buf[100];

    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    printf("Data: %s\n", buf);

    const char* resp_str = "Post data cc";
    httpd_resp_send(req, resp_str, strlen(resp_str));

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t data = {
    .uri       = "/data",
    .method    = HTTP_POST,
    .handler   = data_post_handler,
    .user_ctx  = NULL
};

/* An HTTP POST handler */
static esp_err_t switch_1_post_handler(httpd_req_t *req)
{
    char buf[100];

    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    printf("Switch 1 state: %s\n", (buf[0] == '1') ? "On" : "Off");
    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t switch_1 = {
    .uri       = "/switch_1",
    .method    = HTTP_POST,
    .handler   = switch_1_post_handler,
    .user_ctx  = NULL
};

void start_webserver(void)
{   
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &web);
        httpd_register_uri_handler(server, &hello);
        httpd_register_uri_handler(server, &data);
        httpd_register_uri_handler(server, &dht11_get);
        httpd_register_uri_handler(server, &switch_1);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}
