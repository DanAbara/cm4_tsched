project(tsched)

# add_library(stm32_startup stm32_startup.c)
add_library(led led.c)

add_executable(main_tsched.elf 
    main.c
    stm32_startup.c 
    syscalls.c
)

# target_include_directories(main_tsched.elf ${INCLUDE_DIR})

# target_link_options(main_tsched.elf
#     ${CPU_FLAGS}
#     "--specs=nano.specs"
#     "-T${LINKER_SCRIPT}"
#     "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/main_tsched.map"
# )

# target_link_libraries(main_tsche led)
