# For compiling on Raspberry Pi with WiringPi
#
# Install wiringpi:
#
# wget https://project-downloads.drogon.net/wiringpi-latest.deb
# sudo dpkg -i wiringpi-latest.deb
#
# Build Project:
#
# mkdir build
# cd build
# cmake ..
# make -j4

cmake_minimum_required(VERSION 3.5)
project(MAX3000_Lib)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
add_library(MAX3000_Lib
    src/MAX3000_Pi.h
    src/MAX3000_Pi.cpp
    src/MAX3000_Lib.h
    src/MAX3000_Lib.cpp
)

add_executable(checkerboard
    examples/checkerboard/main.cpp
)
add_definitions("-DWIRINGPI")

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(checkerboard MAX3000_Lib ${WIRINGPI_LIBRARIES})
