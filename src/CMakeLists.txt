project(tsched)

# add_link_options(--specs=nano.specs -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/main_tsched.map)

add_library(led led.c)

# standard executable and link options
add_executable(main_tsched.elf 
    main.c
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
    stm32_startup.c
)

target_link_options(main_tsched_sh.elf PRIVATE 
    "--specs=rdimon.specs" 
    "-T${LINKER_SCRIPT}" 
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/main_tsched_sh.map"
)

target_link_libraries(main_tsched.elf led)
target_link_libraries(main_tsched_sh.elf led rdimon)
