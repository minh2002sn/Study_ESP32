idf_component_register(SRCS "app_http_server.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "esp_http_server" "esp_wifi" "esp_eth"
                    )
