cmake_minimum_required(VERSION 3.5)

project(app_mrf_rpi)

set(CMAKE_CXX_STANDARD 17)

#find_library(BCM2835_LIB bcm2835)

add_executable(app_mrf_rpi_libs main.cpp mrf24j40.cpp spi.cpp gpio.cpp run.cpp spi_not_used.cpp)

target_link_libraries(app_mrf_rpi_libs)

#target_link_libraries(app_25aa ${BCM2835_LIB})
