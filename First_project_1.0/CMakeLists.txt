# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# (Not part of the boilerplate)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/apps ${CMAKE_CURRENT_LIST_DIR}/libraries ${CMAKE_CURRENT_LIST_DIR}/subdrivers)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(first_project)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/setting_wifi_web.html" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "certificates/client.crt" TEXT)
