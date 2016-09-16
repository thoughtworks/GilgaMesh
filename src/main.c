#include "main.h"

#include <nrf_log_ctrl.h>
#include "app/custom_message_types/group_value_message.h"
#include "app/custom_message_types/vote_ack_message.h"
#include "app/custom_message_types/vote_message.h"
#include "app/custom_message_types/voting_heartbeat_message.h"
#include <app/buzzer.h>
#include "app/feedback.h"
#include "app/led.h"
#include "app/nfc.h"
#include "app/storage.h"
#include "message_types/broadcast_message.h"
#include "message_types/handshake_message.h"
#include "message_types/heartbeat_message.h"
#include "system/log.h"
#include "gap.h"
#include "softdevice.h"
#include "terminal.h"

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

#ifndef TESTING
volatile uint32_t m_uicr_bootloader_start_address __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;
#endif

void HardFault_Handler(void) {
  display_failure_feedback();
}

static void init_module(char* module_name, void (*function)()) {
  MESH_LOG_DEBUG("Initializing %s... \r\n", module_name);
  (*function)();
}

static void initialize() {
  NRF_LOG_INIT(NULL);
  MESH_LOG("\r\n[[ MeshyMesh is booting ]]\r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  init_module("softdevice", softdevice_initialize);
  init_module("timer", timer_initialize);
  init_module("terminal", terminal_initialize);
  init_module("commands", command_initialize);
  init_module("leds", led_initialize);
#ifdef IS_PROD_BOARD
  init_module("buzzer", buzzer_initialize);
  init_module("NFC", nfc_initialize);
#endif
  init_module("feedback", feedback_initialize);
  init_module("storage", storage_initialize);
  init_module("votes", votes_initialize);
  init_module("RTC", rtc_init);
  init_module("connections", connections_initialize);
  init_module("GATT", gatt_initialize);
  init_module("GAP", gap_initialize);
  init_module("broadcast messages", broadcast_message_initialize);
  init_module("handshake messages", handshake_message_initialize);
  init_module("heartbeat messages", heartbeat_message_initialize);
  init_module("group-value messages", group_value_message_initialize);
  init_module("vote messages", vote_message_initialize);
  init_module("vote acknowledgement messages", vote_ack_message_initialize);
  init_module("voting heartbeat messages", voting_heartbeat_message_initialize);

  mesh_add_terminal_command("clr", "Clear all votes", clear_all_votes);

  MESH_LOG("System ready.\r\n");
}

static void run() {
  terminal_process_input();
  app_sched_execute();

  EC(sd_app_evt_wait());
}

#ifndef TESTING // exclude entry point from unit tests
int main() {
  initialize();
  display_successful_start_up_feedback();
  //determine if there was a failure and then try to give "failure" feedback
  //display_catastrophic_failure_feedback();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  for(;;) {
    run();
  }
#pragma clang diagnostic pop
}
#endif
