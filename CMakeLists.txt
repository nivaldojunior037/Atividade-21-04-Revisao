# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(AtvRevisao C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(AtvRevisao 
    AtvRevisao.c 
    inc/ssd1306.c      # Arquivo da biblioteca OLED
    AtvRevisao.pio)

pico_set_program_name(AtvRevisao "AtvRevisao")
pico_set_program_version(AtvRevisao "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(AtvRevisao 0)
pico_enable_stdio_usb(AtvRevisao 1)

pico_generate_pio_header(AtvRevisao ${CMAKE_CURRENT_LIST_DIR}/AtvRevisao.pio)

target_sources(AtvRevisao PRIVATE AtvRevisao.c)

# Add the standard library to the build
target_link_libraries(AtvRevisao PRIVATE
        hardware_adc
        hardware_clocks
        hardware_i2c
        hardware_pio
        pico_bootrom
        pico_time
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(AtvRevisao PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(AtvRevisao 
        
        )

pico_add_extra_outputs(AtvRevisao)

