#include "main.h"

#include <app_scheduler.h>
#include "app/buzzer.h"
#include "app/led.h"
#include "app/rtc.h"
#include "message_types/heartbeat_message.h"
#include "connection.h"
#include "gap.h"
#include "softdevice.h"
#include "terminal.h"

// This variable ensures that the linker script will write the bootloader start
// address to the UICR register. This value will be written in the HEX file and
// thus written to UICR when MeshyMesh is flashed into the chip. */

#ifndef TESTING
volatile uint32_t m_uicr_bootloader_start_address  __attribute__ ((section(".uicrBootStartAddress"))) = BOOTLOADER_REGION_START;
#endif

void panic()
{
  NRF_LOG_PRINTF("PANIC!!!\r\n");
  for(;;) { }
}


void initialize() {
  NRF_LOG_INIT();
  NRF_LOG_PRINTF("\r\n[[ MeshyMesh is booting ]]\r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  init_module("Timers", timer_initialize);
  init_module("Softdevice", softdevice_initialize);
  init_module("Terminal", terminal_initialize);
  init_module("Commands", command_initialize);
  init_module("Leds", led_initialize);
  init_module("RTC", rtc_init);
#ifdef IS_PROD_BOARD
  init_module("Buzzer", buzzer_initialize);
#endif
  init_module("Connections", connections_initialize);
  init_module("GATT", gatt_initialize);
  init_module("GAP", gap_initialize);
  init_module("Heartbeat timer", heartbeat_timer_initialize);

  NRF_LOG_PRINTF("System ready.\r\n");
}

init_module(char* module_name, void (*f)()) {
  NRF_LOG_PRINTF("Init %s... \r\n", module_name);
  (*f)();
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

  led_white_off();
  led_green_dim();

  mesh_add_terminal_command("buzz", "buzzer on [dur] [per]", buzzer_on);

  for(;;) {
    run();
  }
}
#endif
