# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/subdriver ${CMAKE_CURRENT_LIST_DIR}/apps)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(wifi_station)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/web.html" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/setting_wifi_web.html" TEXT)