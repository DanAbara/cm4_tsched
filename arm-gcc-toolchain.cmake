set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
    
# Define cross-compiler binaries
find_program(ARM_GCC_COMPILER arm-none-eabi-gcc REQUIRED)
find_program(ARM_GXX_COMPILER arm-none-eabi-g++ REQUIRED)

set(CMAKE_C_COMPILER ${ARM_GCC_COMPILER})
set(CMAKE_CXX_COMPILER ${ARM_GXX_COMPILER})
set(CMAKE_ASM_COMPILER ${ARM_GCC_COMPILER})
    
   # Force static tracking to prevent desktop validation failures
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
