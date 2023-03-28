set(component_srcs "dht.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES "freertos" "driver" "esp_timer"
                       )
