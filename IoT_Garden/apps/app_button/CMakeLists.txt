idf_component_register(SRCS "app_button.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "button" "subdriver_exti" "app_wifi_config"
                    )
