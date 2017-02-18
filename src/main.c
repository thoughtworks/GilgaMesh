#include "main.h"

#include <nrf_log_ctrl.h>
#include "message_types/broadcast_message.h"
#include "message_types/handshake_message.h"
#include "message_types/heartbeat_message.h"
#include "system/log.h"
#include "gap.h"
#include "softdevice.h"
#include "terminal.h"

#include "app/blinky_app.h"

static bool init_failed = false;

void HardFault_Handler(void) {
  // TODO: trace and halt
}

static void init_module(char* module_name, bool (*function)()) {
  if (init_failed) return;

  MESH_LOG_DEBUG("Initializing %s... \r\n", module_name);
  init_failed = !((*function)());
}

static void initialize() {
  NRF_LOG_INIT(NULL);
  MESH_LOG("\r\n[[ GilgaMesh is booting ]]\r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  init_module("timer", timer_initialize);
  init_module("softdevice", softdevice_initialize);
  init_module("terminal", terminal_initialize);
  init_module("commands", command_initialize);
  init_module("connections", connections_initialize);
  init_module("GATT", gatt_initialize);
  init_module("GAP", gap_initialize);
  init_module("broadcast messages", broadcast_message_initialize);
  init_module("handshake messages", handshake_message_initialize);
  init_module("heartbeat messages", heartbeat_message_initialize);

  // Application initialization
  init_module("blinky example", blinky_initialize);

  if (init_failed) {
    // TODO: halt
  } else {
    MESH_LOG("System ready.\r\n");
  }
}

static void run() {
  terminal_process_input();
  app_sched_execute();

  sd_app_evt_wait();
}

#ifndef TESTING // exclude entry point from unit tests
int main() {
  initialize();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  for(;;) {
    run();
  }
#pragma clang diagnostic pop
}
#endif
