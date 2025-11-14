cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Instabov_Eartag)

target_sources(app PRIVATE 
    src/main.c
    src/ble/ble_adv.c
    src/peripherals/my_gpio.c
    src/peripherals/my_adc.c
    src/peripherals/my_temp.c
    src/libraries/fuel_gauge_cr2430.c
)
