set(component_srcs "bh1750fvi.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES "driver" "freertos"
                       )