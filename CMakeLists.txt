cmake_minimum_required(VERSION 3.22)
add_library(std_lib STATIC)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
target_include_directories(std_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/include
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/std_BMI270
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic
    ${CMAKE_CURRENT_SOURCE_DIR}/std_CC1101
    ${CMAKE_CURRENT_SOURCE_DIR}/demo
)
set(config_dir "${CMAKE_CURRENT_SOURCE_DIR}/configs/${STD_LIB_CONFIG}")

file(GLOB_RECURSE config_sources CONFIGURE_DEPENDS
    "${config_dir}/*.c"
)

target_sources(std_lib PRIVATE ${config_sources})
target_include_directories(std_lib BEFORE PUBLIC "${config_dir}")
target_sources(std_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/quarternion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/imu_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/eskf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/std_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/std_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/std_BMI270/std_BMI270.c
    ${CMAKE_CURRENT_SOURCE_DIR}/std_CC1101/std_CC1101.c
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/demo.c
)
target_link_libraries(std_lib
    stm32cubemx
)