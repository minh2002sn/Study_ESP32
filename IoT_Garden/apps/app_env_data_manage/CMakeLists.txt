idf_component_register(SRCS "app_env_data_manage.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "freertos" "json_generator" "json_parser" "app_mqtt"
                    )
