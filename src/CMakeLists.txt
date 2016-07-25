cmake_minimum_required(VERSION 3.2)

# cross compiling configuration
SET(CMAKE_SYSTEM_NAME Generic)
include(CMakeForceCompiler)

# set compiler and paths
CMAKE_FORCE_C_COMPILER(${DEPLOY_PATH}/gcc-arm-none-eabi/bin/arm-none-eabi-gcc GNU)
SET(TOOLS_PATH              ${DEPLOY_PATH}/gcc-arm-none-eabi/bin)
SET(LINKER_SCRIPT           ${LINKER_PATH}/meshymesh.ld)
SET(COMPILER_PATH           ${DEPLOY_PATH}/gcc-arm-none-eabi/)
SET(OUTPUT_FILE_NAME        MeshyMesh)
find_program(LINKER ${TOOLS_PATH}/arm-none-eabi-ld)
find_program(OBJCOPY ${TOOLS_PATH}/arm-none-eabi-objcopy)
find_program(SIZE ${TOOLS_PATH}/arm-none-eabi-size)
find_program(GDB ${TOOLS_PATH}/arm-none-eabi-gdb)
find_program(OBJDUMP ${TOOLS_PATH}/arm-none-eabi-objdump)
find_program(NM ${TOOLS_PATH}/arm-none-eabi-nm)
find_program(GCOV ${TOOLS_PATH}/arm-none-eabi-gcov)
find_program(GPROF ${TOOLS_PATH}/arm-none-eabi-gprof)
find_program(STRIP ${TOOLS_PATH}/arm-none-eabi-strip)
find_program(AS ${TOOLS_PATH}/arm-none-eabi-as)
find_program(READELF ${TOOLS_PATH}/arm-none-eabi-readelf)

# set compiler options
SET(CMAKE_C_FLAGS_DEBUG "-DNRF_LOG_USES_RTT -DBOARD_PCA10028 -DSOFTDEVICE_PRESENT -DNRF51 -D__HEAP_SIZE=0 -DS130 -DBLE_STACK_SUPPORT_REQD -DSWI_DISABLE0 -DBSP_UART_SUPPORT -mcpu=cortex-m0 -mthumb -mabi=aapcs --std=gnu11 -Wall -Os -g3 -mfloat-abi=soft -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-std=gnu11 -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -DNRF_LOG_USES_RTT -DSOFTDEVICE_PRESENT -DS130 -D DEBUG -g -Os -D${TARGET_BOARD} -DNRF51 -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DDEST_BOARD_ID=0")
SET(CMAKE_C_FLAGS_RELEASE "-std=gnu11 -smcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -DNRF_LOG_USES_RTT -DSOFTDEVICE_PRESENT -DS130 -D NDEBUG -Os -D${TARGET_BOARD} -DNRF51 -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DDEST_BOARD_ID=0")
SET(CMAKE_C_FLAGS_MINSIZEREL "-std=gnu11 -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -DNRF_LOG_USES_RTT -DSOFTDEVICE_PRESENT -DS130 -D NDEBUG -Os -D${TARGET_BOARD} -DNRF51 -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DDEST_BOARD_ID=0")

# set linker options
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m0 -mthumb -mabi=aapcs -Og -fmessage-length=0 -fsigned-char -ffunction-sections -flto -fno-move-loop-invariants -Wextra -g3 -T${LINKER_SCRIPT} -Xlinker --gc-sections -Wl,--gc-sections -Wl,-Map,\"${CMAKE_BINARY_DIR}/MeshyMesh.map\" --specs=nano.specs -lgcc -lc -lnosys")
SET(CMAKE_ASM_FLAGS "-mcpu=cortex-m0 -mthumb -x assembler-with-cpp -DBOARD_PCA10028 -DSOFTDEVICE_PRESENT -DNRF51 -DS130 -DBLE_STACK_SUPPORT_REQD -DBSP_DEFINES_ONLY")


# generate outputs macro
macro(GEN_OUTPUTS target)
    get_filename_component(name ${target} NAME_WE)
    #copy elf and map to build output
    add_custom_command(
        OUTPUT ${name}_copy DEPENDS ${target}
        COMMAND cp ${target} ${BUILD_PATH}/${target}
        COMMAND cp ${CMAKE_BINARY_DIR}/${name}.map ${BUILD_PATH}/${name}.map
    )
    add_custom_command(
        OUTPUT ${name}.size DEPENDS ${target}
        COMMAND ${SIZE} ${target}
        COMMAND ${SIZE} --format=berkeley ${target} > ${BUILD_PATH}/${name}.size
    )
    #generate .bin
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_NAME}.bin DEPENDS ${target}
        COMMAND ${OBJCOPY} -O binary ${target} ${BUILD_PATH}/${name}.bin
    )
    #generate .hex
    add_custom_command(
        OUTPUT ${OUTPUT_FILE_NAME}.hex DEPENDS ${target}
        COMMAND ${OBJCOPY} -O ihex ${target} ${BUILD_PATH}/${name}.hex
    )
    #generate .dump
    add_custom_command(
        OUTPUT ${name}.dump DEPENDS ${target}
        COMMAND ${OBJDUMP} -DSC ${target} > ${BUILD_PATH}/${name}.dump
    )
    #generate .header
    add_custom_command(
        OUTPUT ${name}.header DEPENDS ${target}
        COMMAND ${READELF} -h ${target} > ${BUILD_PATH}/${name}.header
    )
    add_custom_target(
        ${name}_outputs ALL DEPENDS ${name}_copy ${name}.size ${name}.bin ${name}.hex ${name}.header #${name}.dump
    )

endmacro(GEN_OUTPUTS)

# includes common to all targets

include_directories(${MeshyMesh_SOURCE_DIR}/inc)
include_directories(${MeshyMesh_SOURCE_DIR}/config)

# includes for nordic nrf51
include_directories(${COMPONENTS}/ble/ble_radio_notification)
include_directories(${COMPONENTS}/ble/common)
include_directories(${COMPONENTS}/ble/ble_db_discovery)
include_directories(${COMPONENTS}/ble/device_manager)
include_directories(${COMPONENTS}/device)
include_directories(${COMPONENTS}/libraries/scheduler)
include_directories(${COMPONENTS}/libraries/timer)
include_directories(${COMPONENTS}/libraries/button)
include_directories(${COMPONENTS}/libraries/util)
include_directories(${COMPONENTS}/libraries/gpiote)
include_directories(${COMPONENTS}/libraries/ppi)
include_directories(${COMPONENTS}/libraries/pwm)
include_directories(${COMPONENTS}/libraries/scheduler)
include_directories(${COMPONENTS}/libraries/uart)
include_directories(${COMPONENTS}/softdevice/common/softdevice_handler)
include_directories(${COMPONENTS}/softdevice/s130/headers)
include_directories(${COMPONENTS}/softdevice/s130/headers/nrf51)
include_directories(${COMPONENTS}/toolchain)
include_directories(${COMPONENTS}/toolchain/arm)
include_directories(${COMPONENTS}/toolchain/gcc)
include_directories(${COMPONENTS}/toolchain/CMSIS/Include)
include_directories(${COMPONENTS}/drivers_nrf/pstorage)
include_directories(${COMPONENTS}/drivers_nrf/hal)
include_directories(${COMPONENTS}/drivers_nrf/config)
include_directories(${COMPONENTS}/drivers_nrf/common)
include_directories(${COMPONENTS}/drivers_nrf/delay)
include_directories(${COMPONENTS}/drivers_nrf/gpiote)
include_directories(${COMPONENTS}/drivers_nrf/uart)
include_directories(${COMPONENTS}/drivers_nrf/ppi)
include_directories(${COMPONENTS}/drivers_nrf/timer)
include_directories(${COMPONENTS}/drivers_nrf/clock)
include_directories(${EXTERNAL}/segger_rtt)
include_directories(${SOFTDEVICE}/include)
include_directories(${SOFTDEVICE}/include/nrf51)

# link directories
link_directories(${LINKER_PATH})
link_directories(${COMPONENTS}/toolchain/gcc)

# add framework sources
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/common/nrf_drv_common.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/delay/nrf_delay.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/gpiote/nrf_drv_gpiote.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/pstorage/pstorage_raw.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/timer/app_timer.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/timer/app_timer_appsh.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/button/app_button.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/util/app_error.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/gpiote/app_gpiote.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/scheduler/app_scheduler.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/util/app_util_platform.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/util/nrf_log.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/softdevice/common/softdevice_handler/softdevice_handler.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/softdevice/common/softdevice_handler/softdevice_handler_appsh.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/toolchain/system_nrf51.c")
list(APPEND MeshyMesh_SOURCES "${EXTERNAL}/segger_rtt/SEGGER_RTT.c")
list(APPEND MeshyMesh_SOURCES "${EXTERNAL}/segger_rtt/SEGGER_RTT_printf.c")

# compile the ARM6 assembly startup code through the C compiler
list(APPEND MeshyMesh_SOURCES "gcc_startup_nrf51.s")
set_property(SOURCE gcc_startup_nrf51.s PROPERTY LANGUAGE C)

# add project sources
list(APPEND MeshyMesh_SOURCES "bsp.c")
list(APPEND MeshyMesh_SOURCES "conversion.c")
list(APPEND MeshyMesh_SOURCES "command.c")
list(APPEND MeshyMesh_SOURCES "terminal.c")
list(APPEND MeshyMesh_SOURCES "main.c")

add_executable(${OUTPUT_FILE_NAME}.elf ${MeshyMesh_SOURCES})
GEN_OUTPUTS(${OUTPUT_FILE_NAME}.elf)

