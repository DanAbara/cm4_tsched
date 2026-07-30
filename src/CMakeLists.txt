project(tsched)

add_library(led led.c)

# standard executable and link options
add_executable(main_tsched.elf 
    main.c
    sched.c
    stm32_startup.c 
    syscalls.c
)

target_link_options(main_tsched.elf PRIVATE
    "--specs=nano.specs" 
    "-T${LINKER_SCRIPT}" 
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/main_tsched.map"
)

# using semihosting to enable printing
add_executable(main_tsched_sh.elf
    main.c
    sched.c
    stm32_startup.c
)

target_link_options(main_tsched_sh.elf PRIVATE 
    "--specs=rdimon.specs" 
    "-T${LINKER_SCRIPT}" 
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/main_tsched_sh.map"
)

# target_include_directories(main_tsched.elf PUBLIC ${CMAKE_SOURCE_DIR}/inc)
# target_include_directories(main_tsched_sh.elf PUBLIC ${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(main_tsched.elf led)
target_link_libraries(main_tsched_sh.elf led rdimon)
