INCLUDE(CMakeForceCompiler)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

#Make sure this is the absolute path to your GNU compiler install folder. Remember to get the version number correct.
CMAKE_FORCE_C_COMPILER("/usr/bin/arm-none-eabi-gcc" GNU)
add_compile_options(-Wno-deprecated-declarations) # Get rid of "The CMAKE_FORCE_C_COMPILER macro is deprecated." warning at compilation

SET(CMAKE_C_FLAGS "-std=gnu99 -mthumb -mabi=aapcs -g3 -mfloat-abi=soft -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums")
SET(CMAKE_ASM_FLAGS "-x assembler-with-cpp")

SET(LINKER_SCRIPT "/home/martin/ClionProjects/weather_thingy_v3/linker_nrf52.ld")
SET(TEMPLATE_PATH "/home/martin/nordic/SDK11/components/toolchain/gcc/Makefile.posix")
SET(CMAKE_EXE_LINKER_FLAGS "-Xlinker -Map=${OUTPUT_FILENAME}.map -mthumb -mabi=aapcs -L ${TEMPLATE_PATH} -T${LINKER_SCRIPT} -Wl,--gc-sections  --specs=nano.specs -lc -lnosys")

