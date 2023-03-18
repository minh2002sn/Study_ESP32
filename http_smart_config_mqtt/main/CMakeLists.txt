idf_component_register(SRCS "app_main.c" "app_wifi_config.c" "app_mqtt.c" "app_nvs.c"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
