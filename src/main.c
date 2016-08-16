#include "main.h"

#include <app_scheduler.h>
#include "app/led.h"
#include "connection.h"
#include "gap.h"
#include "rtc.h"
#include "softdevice.h"
#include "terminal.h"

#define APP_TIMER_PRESCALER         0                                  /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE     4                                  /**< Size of timer operation queues. */

static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);

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


  NRF_LOG_PRINTF("Init scheduler... \r\n");
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

  NRF_LOG_PRINTF("Init timers... \r\n");
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);

  NRF_LOG_PRINTF("Init softdevice... \r\n");
  softdevice_initialize();

  NRF_LOG_PRINTF("Init terminal... \r\n");
  terminal_initialize();

  NRF_LOG_PRINTF("Init command... \r\n");
  command_initialize();

  NRF_LOG_PRINTF("Init leds... \r\n");
  led_initialize();

  NRF_LOG_PRINTF("Init real time clock... \r\n");
  rtc_init();

  NRF_LOG_PRINTF("Init connections... \r\n");
  connections_initialize();

  NRF_LOG_PRINTF("Init GATT... \r\n");
  gatt_initialize();

  NRF_LOG_PRINTF("Init GAP... \r\n");
  gap_initialize();

  NRF_LOG_PRINTF("System ready.\r\n");
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

  led_green_dim();

  for(;;) {
    run();
  }
}
#endif
