## Header
cmake_minimum_required(VERSION 3.6)
project(blinky C ASM)

## Define absolute path to your SDK root folder
set(SDK_ROOT_FOLDER "~/nordic/SDK11")

## Define nRF5x verions
set(NRF_VERSION NRF51)

## Choose defines based on nRF5 version
if (NRF_VERSION MATCHES NRF51)
    # nRF51 defines
    set(SOC_DEFINES
            -DNRF51
            -DBOARD_PCA10028
            -DS130
            )

    # Include nRF51 specific folders
    include_directories(
            config/ble_app_template_s130_pca10028
            ${SDK_ROOT_FOLDER}/components/softdevice/s130/headers
            ${SDK_ROOT_FOLDER}/components/softdevice/s130/headers/nrf51)

    # Include nRF51 specific source files
    set(SOC_SOURCE_FILES
            ${SDK_ROOT_FOLDER}/components/toolchain/gcc/gcc_startup_nrf51.s
            ${SDK_ROOT_FOLDER}/components/toolchain/system_nrf51.c)

    set(SOC_CMAKE_C_FLAGS -mcpu=cortex-m0)
    set(SOC_EXE_LINKER_FLAGS -mcpu=cortex-m0)

    set(LINKER_FILE ${PROJECT_SOURCE_DIR}/linker_nrf51.ld)

    set(SD_FILE ${SDK_ROOT_FOLDER}/components/softdevice/s130/hex/s130_nrf51_2.0.0_softdevice.hex)

    # Set device type to flash
    set(FLASH_SOC nrf51)

elseif (NRF_VERSION MATCHES NRF52)
    # nRF52 defines
    set(SOC_DEFINES
            -DNRF52
            -DBOARD_PCA10040
            -DS132
            -DCONFIG_GPIO_AS_PINRESET
            -DNRF52_PAN_12
            -DNRF52_PAN_15
            -DNRF52_PAN_58
            -DNRF52_PAN_55
            -DNRF52_PAN_54
            -DNRF52_PAN_31
            -DNRF52_PAN_30
            -DNRF52_PAN_51
            -DNRF52_PAN_36
            -DNRF52_PAN_53
            -DNRF52_PAN_20
            -DNRF52_PAN_64
            -DNRF52_PAN_62
            -DNRF52_PAN_63
            )

    # Include nRF52 specific folders
    include_directories(
            config/ble_app_template_s132_pca10040
            ${SDK_ROOT_FOLDER}/components/softdevice/s132/headers
            ${SDK_ROOT_FOLDER}/components/softdevice/s132/headers/nrf52)

    # Include nRF52 specific source files
    set(SOC_SOURCE_FILES
            ${SDK_ROOT_FOLDER}/components/toolchain/gcc/gcc_startup_nrf52.s
            ${SDK_ROOT_FOLDER}/components/toolchain/system_nrf52.c)

    set(SOC_CMAKE_C_FLAGS -mcpu=cortex-m4)
    set(SOC_EXE_LINKER_FLAGS -mcpu=cortex-m4)

    set(SD_FILE ${SDK_ROOT_FOLDER}/components/softdevice/s132/hex/s132_nrf52_2.0.0_softdevice.hex)

    # Set device type to flash
    set(FLASH_SOC nrf52)

endif ()

## Defines common to both nRFx versions
set(COMMON_DEFINES
        -DDEBUG
        -DSOFTDEVICE_PRESENT
        -DNRF_LOG_USES_RTT=1
        -DBLE_STACK_SUPPORT_REQD
        -DSWI_DISABLE0)

add_definitions(${SOC_DEFINES} ${COMMON_DEFINES})

## Include common directories
include_directories(
        config
        debug
        include
        ${SDK_ROOT_FOLDER}/components/toolchain/CMSIS/Include
        ${SDK_ROOT_FOLDER}/components/toolchain/gcc
        ${SDK_ROOT_FOLDER}/components/toolchain/
        ${SDK_ROOT_FOLDER}/components/device
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/delay
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/hal
        ${SDK_ROOT_FOLDER}/examples/bsp
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
        ${SDK_ROOT_FOLDER}/examples/../bsp
        ${SDK_ROOT_FOLDER}/external/segger_rtt
#        ${SDK_ROOT_FOLDER}/components/drivers_nrf/adc
)


## Include all source files
set(COMMON_SOURCE_FILES
        main.c
        ${SDK_ROOT_FOLDER}/components/drivers_nrf/delay/nrf_delay.c
        src/ble_bme280.c
        src/bme280.c
#        ble_battery.c
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
        ${SDK_ROOT_FOLDER}/components/softdevice/common/softdevice_handler/softdevice_handler.c
#        ${SDK_ROOT_FOLDER}/components/drivers_nrf/adc/nrf_drv_adc.c
        )


set(MAIN_TARGET ${PROJECT_NAME}.elf)
add_executable(${MAIN_TARGET} ${COMMON_SOURCE_FILES} ${SOC_SOURCE_FILES} ${LINKER_SCRIPT})


## Set additional project-specific flags for compilers like e.g.:
# '-O0' for Optimization level 0.
# '-Werror' to treat all warnings as error.
# '-lc -lrdimon -u _printf_float' for printf with floating points.
SET(CMAKE_C_FLAGS "${SOC_CMAKE_C_FLAGS} ${CMAKE_C_FLAGS} -O0 -lc -lrdimon -u _printf_float -ggdb3 -fomit-frame-pointer") # <- Add extra project-specific flags for C compiler.
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -ggdb3 -O0 " ) # <- Add extra project-specific flags for ASM compiler.
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SOC_EXE_LINKER_FLAGS}")


# Make binary files
set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${MAIN_TARGET} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${MAIN_TARGET}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${MAIN_TARGET}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}"
        )


# Flash binary to chip
add_custom_command(TARGET ${MAIN_TARGET} POST_BUILD
        COMMAND ./flash.sh ${FLASH_SOC} ${PROJECT_SOURCE_DIR} ${HEX_FILE} ${SD_FILE}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Programming...\n"
        )