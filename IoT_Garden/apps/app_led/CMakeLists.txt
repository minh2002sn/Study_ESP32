idf_component_register(SRCS "app_led.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "subdriver_gpio" "freertos"
                    )
