cmake_minimum_required(VERSION 3.2)

#set common variables
SET(SDK_BASE                ${DEPLOY_PATH}/sdk/nrf_sdk_9_0)
SET(COMPONENTS              ${SDK_BASE}/components)

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

# includes common to all targets

include_directories(${MeshyMesh_SOURCE_DIR}/inc)
include_directories(${MeshyMesh_SOURCE_DIR}/inc/nrf)
include_directories(${MeshyMesh_SOURCE_DIR}/config)

# includes for nordic nrf51
include_directories(${COMPONENTS}/ble/ble_radio_notification)
include_directories(${COMPONENTS}/ble/ble_services/ble_dfu)
include_directories(${COMPONENTS}/ble/common)
include_directories(${COMPONENTS}/device)
include_directories(${COMPONENTS}/libraries/timer)
include_directories(${COMPONENTS}/libraries/button)
include_directories(${COMPONENTS}/libraries/util)
include_directories(${COMPONENTS}/libraries/gpiote)
include_directories(${COMPONENTS}/libraries/bootloader_dfu)
include_directories(${COMPONENTS}/softdevice/common/softdevice_handler)
include_directories(${COMPONENTS}/softdevice/s130/headers)
include_directories(${COMPONENTS}/toolchain)
include_directories(${COMPONENTS}/toolchain/arm)
include_directories(${COMPONENTS}/toolchain/gcc)
include_directories(${COMPONENTS}/drivers_nrf/pstorage)
include_directories(${COMPONENTS}/drivers_nrf/hal)
include_directories(${COMPONENTS}/drivers_nrf/config)
include_directories(${COMPONENTS}/drivers_nrf/common)
include_directories(${COMPONENTS}/drivers_nrf/gpiote)
include_directories(${COMPONENTS}/drivers_nrf/uart)

#set compiler and linker options
SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -g3 -flto -fno-move-loop-invariants -Wextra -Wno-attributes -Wno-old-style-declaration -Og -fno-exceptions -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(RunUnitTests
               unit/GapTest.c
               unit/GattTest.c
               unit/RunTests.c)

target_link_libraries(RunUnitTests cmocka)
GEN_OUTPUTS(RunUnitTests)