idf_component_register(SRCS "http_server_app.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "esp_http_server" "esp_wifi" "esp_eth"
                    )
