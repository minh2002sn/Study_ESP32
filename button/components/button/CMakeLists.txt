set(component_srcs "src/button.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "inc"
                       PRIV_REQUIRES "driver" "esp_timer"
                       )
