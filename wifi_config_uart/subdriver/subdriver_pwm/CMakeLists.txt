set(component_srcs "subdriver_pwm.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES "driver"
                       )