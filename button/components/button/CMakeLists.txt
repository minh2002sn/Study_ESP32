set(component_srcs "button.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES "driver" "esp_timer" "freertos" "subdriver_exti"
                       )
