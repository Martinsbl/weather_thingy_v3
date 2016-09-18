## Header
cmake_minimum_required(VERSION 3.6)
project(blinky C ASM)

## Define absolute path to your SDK root folder
set(SDK_ROOT_FOLDER "~/Nordic/SDK11")

## File includes
add_definitions(-DNRF_LOG_USES_RTT=1
        -DBOARD_PCA10028
        -DSOFTDEVICE_PRESENT
        -DSWI_DISABLE0
        -DNRF51
        -DS130
        -DBLE_STACK_SUPPORT_REQD)

## Include all directories
include_directories(
        ${SDK_ROOT_FOLDER}/components/toolchain/CMSIS/Include
        ${SDK_ROOT_FOLDER}/components/toolchain/gcc
        ${SDK_ROOT_FOLDER}/components/toolchain/
        ${SDK_ROOT_FOLDER}/components/device
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/delay
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/hal
        ${SDK_ROOT_FOLDER}/examples/bsp
        config/ble_app_template_s130_pca10028
        config
        ${SDK_ROOT_FOLDER}/components/ble/ble_advertising
        ${SDK_ROOT_FOLDER}/components/ble/common
        ${SDK_ROOT_FOLDER}/components/ble/device_manager
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/common
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/twi_master
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/config
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/gpiote
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/pstorage
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/uart
        ${SDK_ROOT_FOLDER}/components/libraries/button
        ${SDK_ROOT_FOLDER}/components/libraries/experimental_section_vars
        ${SDK_ROOT_FOLDER}/components/libraries/fstorage
        ${SDK_ROOT_FOLDER}/components/libraries/fstorage/config
        ${SDK_ROOT_FOLDER}/components/libraries/sensorsim
        ${SDK_ROOT_FOLDER}/components/libraries/timer
        ${SDK_ROOT_FOLDER}/components/libraries/trace
        ${SDK_ROOT_FOLDER}/components/libraries/uart
        ${SDK_ROOT_FOLDER}/components/libraries/util
        ${SDK_ROOT_FOLDER}/components/libraries/fifo
        ${SDK_ROOT_FOLDER}/components/softdevice/common/softdevice_handler
        ${SDK_ROOT_FOLDER}/components/softdevice/s130/headers
        ${SDK_ROOT_FOLDER}/components/softdevice/s130/headers/nrf51
        ${SDK_ROOT_FOLDER}/examples/../bsp
        ${SDK_ROOT_FOLDER}/external/segger_rtt
        debug)


## Include all source files
set(SOURCE_FILES
        main.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/delay/nrf_delay.c
        ${SDK_ROOT_FOLDER}/components/toolchain/gcc/gcc_startup_nrf51.s
        ${SDK_ROOT_FOLDER}/components/toolchain/system_nrf51.c
        ble_bme280.c
        debug/msgs.c
        ${SDK_ROOT_FOLDER}/examples/bsp/bsp.c
        ${SDK_ROOT_FOLDER}/examples/bsp/bsp_btn_ble.c
        ${SDK_ROOT_FOLDER}/components/ble/common/ble_advdata.c
        ${SDK_ROOT_FOLDER}/components/ble/ble_advertising/ble_advertising.c
        ${SDK_ROOT_FOLDER}/components/ble/common/ble_conn_params.c
        ${SDK_ROOT_FOLDER}/components/ble/common/ble_srv_common.c
        ${SDK_ROOT_FOLDER}/components/ble/device_manager/device_manager_peripheral.c
        ${SDK_ROOT_FOLDER}/components/libraries/uart/app_uart.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/common/nrf_drv_common.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/gpiote/nrf_drv_gpiote.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/uart/nrf_drv_uart.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/pstorage/pstorage.c
        bme280.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/twi_master/nrf_drv_twi.c
        ${SDK_ROOT_FOLDER}/components/libraries/button/app_button.c
        ${SDK_ROOT_FOLDER}/components/libraries/util/app_error.c
        ${SDK_ROOT_FOLDER}/components/libraries/util/app_error_weak.c
        ${SDK_ROOT_FOLDER}/components/libraries/timer/app_timer.c
        ${SDK_ROOT_FOLDER}/components/libraries/trace/app_trace.c
        ${SDK_ROOT_FOLDER}/components/libraries/util/app_util_platform.c
        ${SDK_ROOT_FOLDER}/components/libraries/fstorage/fstorage.c
        ${SDK_ROOT_FOLDER}/components/libraries/util/nrf_assert.c
        ${SDK_ROOT_FOLDER}/components/libraries/util/nrf_log.c
        ${SDK_ROOT_FOLDER}/components/libraries/uart/retarget.c
        ${SDK_ROOT_FOLDER}/components/libraries/sensorsim/sensorsim.c
        ${SDK_ROOT_FOLDER}/external/segger_rtt/SEGGER_RTT.c
        ${SDK_ROOT_FOLDER}/external/segger_rtt/SEGGER_RTT_printf.c
        ${SDK_ROOT_FOLDER}/components/softdevice/common/softdevice_handler/softdevice_handler.c)


add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})


## Set additional project-specific flags for compilers like e.g.:
# '-O0' for Optimization level 0.
# '-Werror' to treat all warnings as error.
# '-lc -lrdimon -u _printf_float' for printf with floating points.
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0") # <- Add extra project-specific flags for C compiler.
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}") # <- Add extra project-specific flags for ASM compiler.

## Create .hex and .bin files after linking project
set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}"
        )


### Program application onto nRF5
#add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#        COMMAND nrfjprog -e # Erase application region in flash.
#        COMMAND nrfjprog --program ${HEX_FILE} -r # Program application and reset.
#        COMMENT "Programming...\n"
#        )
