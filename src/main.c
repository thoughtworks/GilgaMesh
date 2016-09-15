#include "main.h"

#include <nrf_log_ctrl.h>
#include "app/custom_message_types/group_value_message.h"
#include "app/custom_message_types/vote_ack_message.h"
#include "app/custom_message_types/vote_message.h"
#include "app/buzzer.h"
#include "app/feedback.h"
#include "app/led.h"
#include "app/nfc.h"
#include "app/storage.h"
#include "message_types/heartbeat_message.h"
#include "system/log.h"
#include "connection.h"
#include "gap.h"
#include "softdevice.h"
#include "terminal.h"

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

#ifndef TESTING
volatile uint32_t m_uicr_bootloader_start_address __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;
#endif

void HardFault_Handler(void)
{
  display_failure_feedback();
}

void initialize() {
  NRF_LOG_INIT(NULL);
  MESH_LOG("\r\n[[ MeshyMesh is booting ]]\r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  softdevice_initialize();
  timer_initialize();
  terminal_initialize();
  command_initialize();
  led_initialize();
#ifdef IS_PROD_BOARD
  buzzer_initialize();
  nfc_initialize();
#endif
  feedback_initialize();
  storage_initialize();
  votes_initialize();
  rtc_init();
  connections_initialize();
  gatt_initialize();
  gap_initialize();
  heartbeat_initialize();
  vote_message_initialize();


  mesh_add_terminal_command("vc", "Print current vote configuration", print_current_vote_config);
  mesh_add_terminal_command("grp", "Update group", broadcast_group_value_update);
  mesh_add_terminal_command("val", "Update value", broadcast_group_value_update);
  mesh_add_terminal_command("vote", "Register a vote", save_vote_from_command_line);
  mesh_add_terminal_command("vack", "Broadcast vote ack", broadcast_vote_ack_from_cmd_line);
  add_write_event(Custom, receive_group_value_update);
  add_write_event(5, receive_vote);
  add_write_event(6, receive_vote_acknowledgement);

  MESH_LOG("System ready.\r\n");
}

void run() {
  terminal_process_input();
  app_sched_execute();

  EC(sd_app_evt_wait());
}

#ifndef TESTING // exclude entry point from unit tests
int main()
{
  initialize();
  display_successful_start_up_feedback();
  //determine if there was a failure and then try to give "failure" feedback
  //display_catastrophic_failure_feedback();

  for(;;) {
    run();
  }
}
#endif
