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
SET(CMAKE_C_FLAGS_DEBUG "-DDEBUG -D${TARGET_BOARD} -DNRF_LOG_USES_RTT -DSVCALL_AS_NORMAL_FUNCTION -DBOARD_PCA10028 -DSOFTDEVICE_PRESENT -DBLE_STACK_SUPPORT_REQD -DNRF51 -D__HEAP_SIZE=0 -DS130 -DSWI_DISABLE0 -DBSP_UART_SUPPORT -DTESTING -DNO_LOG_OUTPUT -std=gnu11 -g3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -fno-move-loop-invariants -Wextra -Wno-attributes -Wno-old-style-declaration -Og -fno-exceptions -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

# include cmocka test framework
include_directories(${DEPLOY_PATH}/cmocka/include)
link_directories(${DEPLOY_PATH}/cmocka/lib)

# production code headers
include_directories(${INC_PATH})
include_directories(${CONFIG_PATH})

# includes for real nordic sdk11 headers
include_directories(${COMPONENTS}/ble/ble_db_discovery)
include_directories(${COMPONENTS}/ble/ble_radio_notification)
include_directories(${COMPONENTS}/ble/common)
include_directories(${COMPONENTS}/ble/device_manager)
include_directories(${COMPONENTS}/device)
include_directories(${COMPONENTS}/libraries/button)
include_directories(${COMPONENTS}/libraries/gpiote)
include_directories(${COMPONENTS}/libraries/low_power_pwm)
include_directories(${COMPONENTS}/libraries/ppi)
include_directories(${COMPONENTS}/libraries/pwm)
include_directories(${COMPONENTS}/libraries/scheduler)
include_directories(${COMPONENTS}/libraries/timer)
include_directories(${COMPONENTS}/libraries/uart)
include_directories(${COMPONENTS}/libraries/util)
include_directories(${COMPONENTS}/softdevice/common/softdevice_handler)
include_directories(${COMPONENTS}/toolchain)
include_directories(${COMPONENTS}/toolchain/CMSIS/Include)
include_directories(${COMPONENTS}/toolchain/arm)
include_directories(${COMPONENTS}/toolchain/gcc)
include_directories(${COMPONENTS}/drivers_nrf/clock)
include_directories(${COMPONENTS}/drivers_nrf/common)
include_directories(${COMPONENTS}/drivers_nrf/config)
include_directories(${COMPONENTS}/drivers_nrf/delay)
include_directories(${COMPONENTS}/drivers_nrf/gpiote)
include_directories(${COMPONENTS}/drivers_nrf/hal)
include_directories(${COMPONENTS}/drivers_nrf/ppi)
include_directories(${COMPONENTS}/drivers_nrf/pstorage)
include_directories(${COMPONENTS}/drivers_nrf/timer)
include_directories(${COMPONENTS}/drivers_nrf/twi_master)
include_directories(${COMPONENTS}/drivers_nrf/uart)
include_directories(${EXTERNAL}/segger_rtt)
include_directories(${SOFTDEVICE}/include)
include_directories(${SOFTDEVICE}/include/nrf51)

# include project mock headers
include_directories(mocks/include)

# add sdk11 mock implementations
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/ble_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/ble_gattc_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/ble_gatts_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/ble_l2cap_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/nrf_sdm_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/nrf_soc_mock.c")
list(APPEND TestFramework_SOURCES "mocks/fakeS130_v201/nrf51/nrf_mbr_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/ble/common/ble_conn_state_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/libraries/low_power_pwm/low_power_pwm_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/libraries/scheduler/app_scheduler_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/libraries/timer/app_timer_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/libraries/util/app_error_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/libraries/util/nrf_log_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/softdevice/common/softdevice_handler/softdevice_handler_appsh_mock.c")
list(APPEND TestFramework_SOURCES "mocks/sdk11/components/softdevice/common/softdevice_handler/softdevice_handler_mock.c")

#add system shim mock implementations
list(APPEND TestFramework_SOURCES "mocks/system/delay_mock.c")
list(APPEND TestFramework_SOURCES "mocks/system/gpio_mock.c")
list(APPEND TestFramework_SOURCES "mocks/system/platform_mock.c")
list(APPEND TestFramework_SOURCES "mocks/system/storage_mock.c")
list(APPEND TestFramework_SOURCES "mocks/system/timer_mock.c")

###############################################################################################
# add unit tests and mocks they need

add_executable(BroadcastMessageTest ${TestFramework_SOURCES}
        mocks/connection_mock.c
        mocks/gatt_mock.c
        ${SRC_PATH}/message_types/broadcast_message.c
        unit/message_types/BroadcastMessageTest.c)
target_link_libraries(BroadcastMessageTest cmocka)
GEN_OUTPUTS(BroadcastMessageTest)

add_executable(ConnectionTest ${TestFramework_SOURCES}
        mocks/app/led_mock.c
        mocks/device_mock.c
        mocks/gap_mock.c
        mocks/gatt_mock.c
        ${SRC_PATH}/conversion.c
        ${SRC_PATH}/connection.c
        unit/ConnectionTest.c)
target_link_libraries(ConnectionTest cmocka)
GEN_OUTPUTS(ConnectionTest)

add_executable(CommandTest ${TestFramework_SOURCES}
        mocks/message_types/broadcast_message_mock.c
        mocks/message_types/heartbeat_message_mock.c
        mocks/connection_mock.c
        mocks/gap_mock.c
        mocks/gatt_mock.c
        mocks/terminal_mock.c
        mocks/system/rtc_mock.c
        ${SRC_PATH}/command.c
        unit/CommandTest.c)
target_link_libraries(CommandTest cmocka)
GEN_OUTPUTS(CommandTest)

add_executable(HandshakeMessageTest ${TestFramework_SOURCES}
        mocks/connection_mock.c
        mocks/device_mock.c
        mocks/gatt_mock.c
        ${SRC_PATH}/message_types/handshake_message.c
        unit/message_types/HandshakeMessageTest.c)
target_link_libraries(HandshakeMessageTest cmocka)
GEN_OUTPUTS(HandshakeMessageTest)

add_executable(HeartbeatMessageTest ${TestFramework_SOURCES}
        mocks/connection_mock.c
        mocks/device_mock.c
        mocks/gatt_mock.c
        mocks/system/timer_mock.c
        ${SRC_PATH}/conversion.c
        ${SRC_PATH}/message_types/heartbeat_message.c
        unit/message_types/HeartbeatMessageTest.c)
target_link_libraries(HeartbeatMessageTest cmocka)
GEN_OUTPUTS(HeartbeatMessageTest)

add_executable(GattTest ${TestFramework_SOURCES}
        mocks/message_types/broadcast_message_mock.c
        mocks/message_types/heartbeat_message_mock.c
        mocks/message_types/handshake_message_mock.c
        mocks/connection_mock.c
        mocks/gap_mock.c
        mocks/queue_mock.c
        mocks/system/timer_mock.c
        ${SRC_PATH}/gatt.c
        unit/GattTest.c)
target_link_libraries(GattTest cmocka)
GEN_OUTPUTS(GattTest)

add_executable(LedTest ${TestFramework_SOURCES}
        ${SRC_PATH}/app/led.c
        unit/app/LedTest.c)
target_link_libraries(LedTest cmocka)
GEN_OUTPUTS(LedTest)

add_executable(MainTest ${TestFramework_SOURCES}
        mocks/message_types/heartbeat_message_mock.c
        mocks/connection_mock.c
        mocks/command_mock.c
        mocks/error_mock.c
        mocks/gap_mock.c
        mocks/gatt_mock.c
        mocks/softdevice_mock.c
        mocks/terminal_mock.c
        mocks/system/timer_mock.c
        mocks/app/led_mock.c
        mocks/app/rtc_mock.c
        ${SRC_PATH}/conversion.c
        ${SRC_PATH}/main.c
        unit/MainTest.c)
target_link_libraries(MainTest cmocka)
GEN_OUTPUTS(MainTest)

#add_executable(RTCTest ${TestFramework_SOURCES}
#    mocks/terminal_mock.c
#    ${SRC_PATH}/conversion.c
#    ${SRC_PATH}/app/rtc.c
#    unit/app/RTCTest.c)
#target_link_libraries(RTCTest cmocka)
#GEN_OUTPUTS(RTCTest)

add_executable(QueueTest ${TestFramework_SOURCES}
        ${SRC_PATH}/queue.c
        unit/QueueTest.c)
target_link_libraries(QueueTest cmocka)
GEN_OUTPUTS(QueueTest)
