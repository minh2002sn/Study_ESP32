idf_component_register(SRCS "app_illuminance.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "app_env_data_manage" "freertos" "bh1750fvi"
                    )
