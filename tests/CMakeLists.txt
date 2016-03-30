cmake_minimum_required(VERSION 3.2)

# generate outputs macro
macro(GEN_OUTPUTS target)
    get_filename_component(name ${target} NAME_WE)
    #copy test executables
    add_custom_command(
        OUTPUT ${name}_copy_tests DEPENDS ${target}
        COMMAND mkdir -p ${BUILD_PATH}/tests
        COMMAND cp ${target} ${BUILD_PATH}/tests/${target}
    )
    add_custom_target(
        ${name}_outputs ALL DEPENDS ${name}_copy_tests
    )
endmacro(GEN_OUTPUTS)

#set compiler and linker options
SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -D${TARGET_BOARD} -DNRF51 -DDEST_BOARD_ID=0 -DTESTING -std=c11 -g3 -flto -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -fno-move-loop-invariants -Wextra -Wno-attributes -Wno-old-style-declaration -Og -fno-exceptions -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage -L${EHAL_PATH}/ARM/src -L${EHAL_PATH}/ARM/Nordic/nRF51/CMSIS/Debug")

# include mocks
include_directories(${INC_PATH})
include_directories(${CONFIG_PATH})
include_directories(mocks)
include_directories(mocks/sdk)
include_directories(mocks/sdk/s130)

# includes for nordic nrf51
#include_directories(${COMPONENTS}/ble/ble_radio_notification)
#include_directories(${COMPONENTS}/ble/ble_services/ble_dfu)
#include_directories(${COMPONENTS}/ble/common)
#include_directories(${COMPONENTS}/device)
#include_directories(${COMPONENTS}/libraries/timer)
#include_directories(${COMPONENTS}/libraries/button)
#include_directories(${COMPONENTS}/libraries/util)
#include_directories(${COMPONENTS}/libraries/gpiote)
#include_directories(${COMPONENTS}/libraries/bootloader_dfu)
#include_directories(${COMPONENTS}/softdevice/common/softdevice_handler)
#include_directories(${COMPONENTS}/softdevice/s130/headers)
#include_directories(${COMPONENTS}/toolchain)
#include_directories(${COMPONENTS}/toolchain/arm)
#include_directories(${COMPONENTS}/toolchain/gcc)
#include_directories(${COMPONENTS}/drivers_nrf/pstorage)
#include_directories(${COMPONENTS}/drivers_nrf/hal)
#include_directories(${COMPONENTS}/drivers_nrf/config)
#include_directories(${COMPONENTS}/drivers_nrf/common)
#include_directories(${COMPONENTS}/drivers_nrf/gpiote)
#include_directories(${COMPONENTS}/drivers_nrf/uart)

# add framework sources
#list(APPEND RunUnitTests_SOURCES "${EHAL_PATH}/ARM/Nordic/nRF51/src/Vectors_nRF51.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/libraries/timer/app_timer.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/libraries/util/app_util_platform.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/libraries/gpiote/app_gpiote.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/drivers_nrf/gpiote/nrf_drv_gpiote.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/drivers_nrf/common/nrf_drv_common.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/ble/ble_radio_notification/ble_radio_notification.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/drivers_nrf/hal/nrf_delay.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/drivers_nrf/pstorage/pstorage.c")
#list(APPEND RunUnitTests_SOURCES "${COMPONENTS}/softdevice/common/softdevice_handler/softdevice_handler.c")

# add system under test sources
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/gap.c")

#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/nrf/simple_uart.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/connection.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/gatt.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/led.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/logger.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/terminal.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/timer.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/error.c")
#list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/dfu.c")

# add testcase sources
list(APPEND RunUnitTests_SOURCES "unit/GapTest.c")
list(APPEND RunUnitTests_SOURCES "unit/GattTest.c")
list(APPEND RunUnitTests_SOURCES "unit/RunTests.c")


add_executable(RunUnitTests ${RunUnitTests_HEADERS} ${RunUnitTests_SOURCES} )

target_link_libraries(RunUnitTests cmocka)
GEN_OUTPUTS(RunUnitTests)