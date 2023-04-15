idf_component_register(SRCS "app_soil_moisture.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "app_env_data_manage" "freertos" "spi_adc"
                    )
