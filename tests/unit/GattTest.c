#include "gatt.h"
#include "cmocka_includes.h"

static uint16_t handle = 123;

static MessagePropagationType custom_write_event_handler(uint16_t connectionHandle, uint8_t *dataPacket) {
  check_expected(connectionHandle);
  check_expected_ptr(dataPacket);
  return DoNotPropagate;
}

static void Gatt_handle_write_event_handles_custom_message_types() {
  BleMessageType newMessageType = register_message_type(custom_write_event_handler);
  uint8_t newEventMessageData[1] = { newMessageType };

  ble_evt_t bleEvent;
  bleEvent.evt.gap_evt.conn_handle = handle;
  memcpy(bleEvent.evt.gatts_evt.params.write.data, newEventMessageData, sizeof(uint8_t));

  expect_value(custom_write_event_handler, connectionHandle, handle);
  expect_memory(custom_write_event_handler, dataPacket, newEventMessageData, sizeof(uint8_t));
  handle_write_event(&bleEvent, 0);
}

static void Gatt_handle_write_event_does_not_handle_unknown_message_types() {
  uint8_t newEventMessageData[1] = { 99 };

  ble_evt_t bleEvent;
  bleEvent.evt.gap_evt.conn_handle = handle;
  memcpy(bleEvent.evt.gatts_evt.params.write.data, newEventMessageData, sizeof(uint8_t));

  // no expectations: nothing should happen
  handle_write_event(&bleEvent, 0);
}

int RunGattTest(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(Gatt_handle_write_event_handles_custom_message_types),
            cmocka_unit_test(Gatt_handle_write_event_does_not_handle_unknown_message_types),
    };

    return cmocka_run_group_tests_name("GattTest", tests, NULL, NULL);
}

int main() {
    return RunGattTest();
}