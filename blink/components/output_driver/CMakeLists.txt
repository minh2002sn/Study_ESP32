set(component_srcs "src/output_driver.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "inc"
                       PRIV_REQUIRES "driver"
                       )