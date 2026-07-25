set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/arm-gcc-toolchain.cmake")

cmake_minimum_required(VERSION 3.22)
project(tsched)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(MACH "cortex-m4")
set(CPU_FLAGS "-mcpu=${MACH} -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -std=gnu11 -O0 -Wall -g")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stm32_ls.ld")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

add_subdirectory(src)
add_subdirectory(inc)

