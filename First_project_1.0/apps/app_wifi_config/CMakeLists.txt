idf_component_register(SRCS "app_wifi_config.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "esp_wifi" "esp_eth" "app_http_server"
                    )
