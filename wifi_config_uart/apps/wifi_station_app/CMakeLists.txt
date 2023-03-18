idf_component_register(SRCS "wifi_station_app.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "esp_wifi" "esp_http_server" "lwip"
                    )
