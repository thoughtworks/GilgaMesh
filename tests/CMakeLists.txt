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
SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -D${TARGET_BOARD} -DNRF51 -DDEST_BOARD_ID=0 -DTESTING -std=c11 -g3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -fno-move-loop-invariants -Wextra -Wno-attributes -Wno-old-style-declaration -Og -fno-exceptions -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

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

# include project mock headers
include_directories(mocks/config)
include_directories(mocks)

# add sdk mock implementations
list(APPEND TestFramework_SOURCES "mocks/sdk/s130/ble_gap_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk/s130/ble_gatts_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk/s130/ble_gattc_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk/softdevice_handler_mock.c")

###############################################################################################
# add unit tests and mocks they need

add_executable(GapTest ${TestFramework_SOURCES}
    mocks/config/pstorage_platform_mock.c
    mocks/logger_mock.c
    mocks/error_mock.c
    mocks/connection_mock.c
    mocks/gatt_mock.c
    ${SRC_PATH}/gap.c
    unit/GapTest.c)
target_link_libraries(GapTest cmocka)
GEN_OUTPUTS(GapTest)

add_executable(GattTest ${TestFramework_SOURCES}
    mocks/logger_mock.c
    mocks/error_mock.c
    mocks/connection_mock.c
    ${SRC_PATH}/gatt.c
    unit/GattTest.c)
target_link_libraries(GattTest cmocka)
GEN_OUTPUTS(GattTest)

add_executable(ConnectionTest ${TestFramework_SOURCES}
    mocks/logger_mock.c
    mocks/led_mock.c
    mocks/gatt_mock.c
    ${SRC_PATH}/connection.c
    unit/ConnectionTest.c)
target_link_libraries(ConnectionTest cmocka)
GEN_OUTPUTS(ConnectionTest)

