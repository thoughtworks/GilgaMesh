# enable cross compile
SET(CMAKE_SYSTEM_NAME Generic)
include(CMakeForceCompiler)

# set target hardware type
SET(TARGET_BOARD       BOARD_PCA10028)

# set compiler and paths
CMAKE_FORCE_C_COMPILER(${DEPLOY_PATH}/gcc-arm-none-eabi/bin/arm-none-eabi-gcc GNU)
SET(SDK_BASE                ${DEPLOY_PATH}/sdk/nrf_sdk_9_0)
SET(COMPONENTS              ${SDK_BASE}/components)
SET(EHAL_PATH               ${DEPLOY_PATH}/EHAL)
SET(LINKER_SCRIPT           ${LINKER_PATH}/gcc_nrf51_s130_32kb.ld)

# set compiler options
SET(CMAKE_C_FLAGS_DEBUG "-mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -D DEBUG -g -O0 -D${TARGET_BOARD} -DNRF51 -D__need___va_list -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DDEST_BOARD_ID=0")
SET(CMAKE_C_FLAGS_RELEASE  "-mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto -fno-move-loop-invariants -Wextra -g3 -DBLE_STACK_SUPPORT_REQD -D NDEBUG -O3 -D${TARGET_BOARD} -DNRF51 -D__need___va_list -w -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -DDEST_BOARD_ID=0")

# set linker options
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -flto -fno-move-loop-invariants -Wextra -g3 -T${LINKER_SCRIPT} -Xlinker --gc-sections -Wl,-Map,\"${CMAKE_BINARY_DIR}/MeshyMesh.map\" --specs=nano.specs -L${EHAL_PATH}/ARM/src -L${EHAL_PATH}/ARM/Nordic/nRF51/CMSIS/Debug -lCMSIS")

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

# add framework sources
list(APPEND MeshyMesh_SOURCES "${EHAL_PATH}/ARM/Nordic/nRF51/src/Vectors_nRF51.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/timer/app_timer.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/util/app_util_platform.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/libraries/gpiote/app_gpiote.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/gpiote/nrf_drv_gpiote.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/common/nrf_drv_common.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/ble/ble_radio_notification/ble_radio_notification.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/hal/nrf_delay.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/drivers_nrf/pstorage/pstorage.c")
list(APPEND MeshyMesh_SOURCES "${COMPONENTS}/softdevice/common/softdevice_handler/softdevice_handler.c")

# add project sources
list(APPEND MeshyMesh_SOURCES "nrf/simple_uart.c")
list(APPEND MeshyMesh_SOURCES "connection.c")
list(APPEND MeshyMesh_SOURCES "gap.c")
list(APPEND MeshyMesh_SOURCES "gatt.c")
list(APPEND MeshyMesh_SOURCES "led.c")
list(APPEND MeshyMesh_SOURCES "logger.c")
list(APPEND MeshyMesh_SOURCES "terminal.c")
list(APPEND MeshyMesh_SOURCES "timer.c")
list(APPEND MeshyMesh_SOURCES "error.c")
list(APPEND MeshyMesh_SOURCES "dfu.c")
list(APPEND MeshyMesh_SOURCES "main.c")

# main executable
add_executable(MeshyMesh.elf ${MeshyMesh_SOURCES})

