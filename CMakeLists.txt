cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(sig_gen
        LANGUAGES ASM C CXX
        VERSION 1.0
        DESCRIPTION "Pico Signal Generator"
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(sig_gen
	siggen.cpp
	)
	
pico_generate_pio_header(sig_gen ${CMAKE_CURRENT_LIST_DIR}/clk.pio)

target_sources(sig_gen
    PUBLIC
        ../SSD1306.cpp
        ../GFX.cpp
	../logo.hpp
)

target_link_libraries(sig_gen
  pico_stdlib
  hardware_i2c
  pico_multicore
  hardware_pio
)

pico_enable_stdio_usb(sig_gen 1)
pico_enable_stdio_uart(sig_gen 0)

pico_add_extra_outputs(sig_gen)
