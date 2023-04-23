idf_component_register(SRCS "app_dht.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "dht" "app_env_data_manage" "freertos"
                    )
