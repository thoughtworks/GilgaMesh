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
SET(CMAKE_C_FLAGS_DEBUG "-DGAP_TEST -DDEBUG -D${TARGET_BOARD} -DNRF51 -DDEST_BOARD_ID=0 -DTESTING -std=c11 -g3 -flto -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -fno-move-loop-invariants -Wextra -Wno-attributes -Wno-old-style-declaration -Og -fno-exceptions -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage -L${EHAL_PATH}/ARM/src -L${EHAL_PATH}/ARM/Nordic/nRF51/CMSIS/Debug")

# production code headers
include_directories(${INC_PATH})
include_directories(${INC_PATH}/nrf)
include_directories(${CONFIG_PATH})

# include SDK mock headers
include_directories(mocks/sdk)
include_directories(mocks/sdk/s130)
include_directories(mocks/sdk/EHAL)
include_directories(mocks/sdk/device)
include_directories(mocks/sdk/toolchain)

# add sdk mock implementations
list(APPEND RunUnitTests_SOURCES "mocks/sdk/s130/ble_gap_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/sdk/softdevice_handler_mock.c")

# include project mock headers
include_directories(mocks/config)
include_directories(mocks)

# add mock implementations
list(APPEND RunUnitTests_SOURCES "mocks/config/pstorage_platform_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/connection_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/dfu_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/error_mock.c")
#list(APPEND RunUnitTests_SOURCES "mocks/gap_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/gatt_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/led_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/logger_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/terminal_mock.c")
list(APPEND RunUnitTests_SOURCES "mocks/timer_mock.c")

# add system under test sources
list(APPEND RunUnitTests_SOURCES "${SRC_PATH}/gap.c")

# add testcase sources
list(APPEND RunUnitTests_SOURCES "unit/GapTest.c")
list(APPEND RunUnitTests_SOURCES "unit/GattTest.c")
list(APPEND RunUnitTests_SOURCES "unit/RunTests.c")

add_executable(RunUnitTests ${RunUnitTests_SOURCES} )

target_link_libraries(RunUnitTests cmocka)
GEN_OUTPUTS(RunUnitTests)