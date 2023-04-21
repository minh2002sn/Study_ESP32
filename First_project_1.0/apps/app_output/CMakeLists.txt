idf_component_register(SRCS "app_output.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "app_env_data_manage" "freertos" "subdriver_gpio" "subdriver_pwm" "driver"
                    )
