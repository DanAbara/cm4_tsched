# Set GNU Arm toolchain
set(CMAKE_SYSTEM_MAKE Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(ARM_GCC_COMPILER arm-none-eabi-gcc REQUIRED)

set(CMAKE_C_COMPILER ${ARM_GCC_COMPILER})
set(CMAKE_ASM_COMPILER ${ARM_GCC_COMPILER})

# Force static tracking to prevent desktop validation failures
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# initialize project
cmake_minimum_required(VERSION 3.22)
project(tsched)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# set CPU flags
set(CPU_FLAGS "-mthumb -mfloat-abi=soft -mcpu=cortex-m4")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -std=gnu11 -O0 -Wall -g")

# linker script
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stm32_ls.ld")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_subdirectory(src)
add_subdirectory(inc)