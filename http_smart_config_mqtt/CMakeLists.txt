# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# (Not part of the boilerplate)
# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
# set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/apps ${CMAKE_CURRENT_LIST_DIR}/libraries)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(http_smart_config_mqtt)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/setting_wifi_web.html" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.key" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/mosquitto.org.crt" TEXT)
