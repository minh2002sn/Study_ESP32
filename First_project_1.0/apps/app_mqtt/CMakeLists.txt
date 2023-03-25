idf_component_register(SRCS "app_mqtt.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "esp_wifi" "mqtt" "log"
                    )
