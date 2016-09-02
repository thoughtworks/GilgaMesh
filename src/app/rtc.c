#include <app/rtc.h>
#include "conversion.h"
#include "logger.h"
#include "terminal.h"
#include "system/timer.h"

#define MS_RATE_TO_UPDATE_SYSTEM_CLOCK 1
static bool is_sysclock_suspended = false;
APP_TIMER_DEF(SYSCLOCK_TIMER); // register timer that updates the system clock

#ifdef SEEED_RTC
#include <app_twi.h>
#include <app_util_platform.h>

#define MAX_PENDING_TRANSACTIONS 8
#define RTC_TWI_ADDR 81

static app_twi_t rtcTwiInstance = APP_TWI_INSTANCE(0);
static bool is_seeed_rtc_connected = false;

static PCF85063TPState_t rtc_state;



static void twi_event_handler(nrf_drv_twi_evt_t *twiEvent) {
  if(twiEvent->type == NRF_DRV_TWI_EVT_DONE) {
    NRF_LOG_INFO("I2C: Transfer completed address: %d length: %d\r\n",
      twiEvent->xfer_desc.address, twiEvent->xfer_desc.primary_length);
  }
  else if(twiEvent->type == NRF_DRV_TWI_EVT_ADDRESS_NACK) {
    NRF_LOG_INFO("I2C: Address error\r\n");
  }
  else if(twiEvent->type == NRF_DRV_TWI_EVT_DATA_NACK) {
    NRF_LOG_INFO("I2C: Data error\r\n");
  }
  else {
    NRF_LOG_INFO("I2C: Unknown error\r\n");
  }
}

void rtc_i2c_read_callback(uint32_t result, void* user_data)
{
  UNUSED_PARAMETER(user_data);

  if (result != NRF_SUCCESS)
  {
    is_seeed_rtc_connected = false;

    NRF_LOG_INFO("I2C: Seeed RTC error: %d\r\n", (int)result);
    return;
  }
  else
  {
    is_seeed_rtc_connected = true;
  }
}

static void read_seeed_rtc_state() {

  uint32_t err_code;

  static app_twi_transfer_t const readRtcTransfers[] =
    {
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x0], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_0.Control_1, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x1], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_1.Control_2, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x2], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_2.Offset, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x3], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_3.RAM_byte, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x4], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_4.Seconds, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x5], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_5.Minutes, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x6], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_6.Hours, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x7], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_7.Days, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x8], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_8.Weekdays, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0x9], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_9.Months, 1, 0),
      APP_TWI_WRITE(RTC_TWI_ADDR, &rtc_register_indices[0xA], 1, APP_TWI_NO_STOP),
      APP_TWI_READ(RTC_TWI_ADDR, &rtc_state.register_A.Years, 1, 0)
    };

    static app_twi_transaction_t const readRtcTransaction =
      {
        .callback = rtc_i2c_read_callback,
        .p_user_data = NULL,
        .p_transfers = readRtcTransfers,
        .number_of_transfers = sizeof(readRtcTransfers) / sizeof(readRtcTransfers[0])
      };

    APP_ERROR_CHECK(app_twi_schedule(&rtcTwiInstance, &readRtcTransaction));
}

static void print_seeed_rtc_state() {
  read_seeed_rtc_state();

  if(is_seeed_rtc_connected) {
    NRF_LOG_INFO("RTC Control_1 CS:%d 12H:%d CIE:%d SR:%d STOP:%d TEST:%d\r\n", rtc_state.register_0.CAP_SEL,
        rtc_state.register_0.AM_PM,
        rtc_state.register_0.CIE,
        rtc_state.register_0.SR,
        rtc_state.register_0.STOP,
        rtc_state.register_0.EXT_TEST);
    NRF_LOG_INFO("RTC Control_2 COF:%d TF:%d HMI:%d MI:%d\r\n", rtc_state.register_1.COF,
        rtc_state.register_1.TF,
        rtc_state.register_1.HMI,
        rtc_state.register_1.MI);
    NRF_LOG_INFO("RTC Offset: %d\r\n", rtc_state.register_2.Offset);
    NRF_LOG_INFO("RTC RAM_byte: %d\r\n", rtc_state.register_3.RAM_byte);
    NRF_LOG_INFO("RTC Seconds OS:%d SECONDS:%d%d\r\n", rtc_state.register_4.OS,
        rtc_state.register_4.TENS,
        rtc_state.register_4.UNIT);
    NRF_LOG_INFO("RTC Minutes: %d%d\r\n", rtc_state.register_5.TENS,
        rtc_state.register_5.UNIT);
    NRF_LOG_INFO("RTC Hours: %d%d\r\n", rtc_state.register_6.TENS,
        rtc_state.register_6.UNIT);
    NRF_LOG_INFO("RTC Days: %d%d\r\n", rtc_state.register_7.TENS,
        rtc_state.register_7.UNIT);
    NRF_LOG_INFO("RTC Weekdays: %d\r\n", rtc_state.register_8.UNIT);
    NRF_LOG_INFO("RTC Months: %d%d\r\n", rtc_state.register_9.TENS,
        rtc_state.register_9.UNIT);
    NRF_LOG_INFO("RTC Years: 20%d%d\r\n", rtc_state.register_A.TENS,
        rtc_state.register_A.UNIT);
  }
};

static void seeed_rtc_init() {
  uint32_t err_code;
  NRF_LOG_INFO("Init Seeed RTC...\r\n");

  nrf_drv_twi_config_t const twi_config = {
    .scl                = 14,
    .sda                = 15,
    .frequency          = NRF_TWI_FREQ_100K,
    .interrupt_priority = APP_IRQ_PRIORITY_LOW
  };

  APP_TWI_INIT(&rtcTwiInstance, &twi_config, MAX_PENDING_TRANSACTIONS, err_code);
  APP_ERROR_CHECK(err_code);

  memset(&rtc_state, 0, sizeof(rtc_state));
  read_seeed_rtc_state();
}

static void update_system_time_from_seeed_rtc(void * p_event_data, uint16_t event_size) {
  UNUSED_PARAMETER(p_event_data);
  UNUSED_PARAMETER(event_size);

  read_seeed_rtc_state();

  system_time.clock_time.seconds = rtc_state.register_4.TENS * 10 + rtc_state.register_4.UNIT;
  system_time.clock_time.minutes = rtc_state.register_5.TENS * 10 + rtc_state.register_5.UNIT;
  system_time.clock_time.hours = rtc_state.register_6.TENS * 10 + rtc_state.register_6.UNIT;
  system_time.clock_time.day = rtc_state.register_7.TENS * 10 + rtc_state.register_7.UNIT;
  system_time.clock_time.weekday = rtc_state.register_8.UNIT;
  system_time.clock_time.month = rtc_state.register_9.TENS * 10 + rtc_state.register_9.UNIT;
  system_time.clock_time.years_since_y2k = rtc_state.register_A.TENS * 10 + rtc_state.register_A.UNIT;
  system_time.clock_time.year = 2000 + system_time.clock_time.years_since_y2k;
  system_time.clock_time.UTC_offset_in_minutes = rtc_state.register_2.Offset * 30;
}

#endif // SEEED_RTC

void rtc_sysclock_timer_initialize() {
  create_repeated_timer(&SYSCLOCK_TIMER);
  start_timer(&SYSCLOCK_TIMER, MS_RATE_TO_UPDATE_SYSTEM_CLOCK, rtc_periodic_update_handler);
}

void rtc_sysclock_timer_suspend() {
  if(!is_sysclock_suspended) {
    is_sysclock_suspended = true;
    stop_timer(&SYSCLOCK_TIMER);
  }
}

void rtc_sysclock_timer_resume() {
  if(is_sysclock_suspended) {
    is_sysclock_suspended = false;
    start_timer(&SYSCLOCK_TIMER, MS_RATE_TO_UPDATE_SYSTEM_CLOCK, rtc_periodic_update_handler);
  }
}


void rtc_init() {
  system_time.clock_time.day = 1;
  system_time.clock_time.month = 1;
  system_time.clock_time.year = 2000;

  for(int i = 0 ; i < NUMBER_OF_RTC_REGISTERS ; i++) {
    rtc_register_indices[i] = i;
  }

#ifdef SEEED_RTC
  seeed_rtc_init();
#endif // SEEED_RTC

  rtc_sysclock_timer_initialize();

  mesh_add_terminal_command("rtc",  "Real time clock commands", execute_rtc_command);
}

clock_time_t rtc_get_current_time() {
  return system_time.clock_time;
}

void rtc_set_current_time(clock_time_t time) {
}

uint64_t rtc_get_seconds_since_boot() {
  return system_time.seconds_since_boot;
}

void rtc_periodic_update_handler() {
  system_time.clock_time.millis++;
  if(system_time.clock_time.millis >= 1000) {
    system_time.clock_time.millis = 0;
    system_time.seconds_since_boot++;
  }

#ifdef SEEED_RTC
  if(system_time.clock_time.millis % 250 == 0) { // read RTC 4 times per second
    app_sched_event_put(NULL, 0, update_system_time_from_seeed_rtc);
  }
# else // sloppy approximation of time and date if no RTC hardware..
  if(system_time.clock_time.millis == 0) {
    system_time.clock_time.seconds++;
    if(system_time.clock_time.seconds >= 60) {
      system_time.clock_time.seconds = 0;
      system_time.clock_time.minutes++;
      if(system_time.clock_time.minutes >= 60) {
        system_time.clock_time.minutes = 0;
        system_time.clock_time.hours++;
        if(system_time.clock_time.hours >= 24) {
          system_time.clock_time.hours = 0;
          system_time.clock_time.day++;
          if(system_time.clock_time.day >= 31) {
            system_time.clock_time.day = 1;
            system_time.clock_time.month++;
            if(system_time.clock_time.month >= 13) {
              system_time.clock_time.month = 1;
              system_time.clock_time.years_since_y2k++;
              system_time.clock_time.year = 2000 + system_time.clock_time.years_since_y2k;
            }
          }
        }
      }
    }
  }
#endif
}

void rtc_print_date_and_time() {
  NRF_LOG_INFO("System time is: %d/%d/%d %02d:%02d:%02d\r\n", system_time.clock_time.month
                                                                              , system_time.clock_time.day
                                                                              , system_time.clock_time.year
                                                                              , system_time.clock_time.hours
                                                                              , system_time.clock_time.minutes
                                                                              , system_time.clock_time.seconds);


  timestamp_t timestamp;
  timestring_t timestring;
  rtc_get_timestamp(timestamp);
  rtc_timestamp_to_timestring(timestamp, timestring);
  NRF_LOG_INFO("Timestamp: %s\r\n", timestring);
}

void rtc_set_field(char* field, char* value) {
#ifdef SEEED_RTC

  uint32_t err_code = 0;
  uint8_t register_to_update = 0;
  uint8_t min = 0;
  uint8_t max = 0;

  if (strcmp(field, "year") == 0) {
    register_to_update = 0xA;
    min = 0;
    max = 99;
  }
  else if (strcmp(field, "month") == 0) {
    register_to_update = 0x9;
    min = 1;
    max = 12;
  }
  else if (strcmp(field, "weekday") == 0) {
    register_to_update = 0x8;
    min = 0;
    max = 6;
  }
  else if (strcmp(field, "day") == 0) {
    register_to_update = 0x7;
    min = 1;
    max = 31;
  }
  else if (strcmp(field, "hour") == 0) {
    register_to_update = 0x6;
    min = 0;
    max = 23;
  }
  else if (strcmp(field, "minute") == 0) {
    register_to_update = 0x5;
    min = 0;
    max = 59;
  }
  else if (strcmp(field, "second") == 0) {
    register_to_update = 0x4;
    min = 0;
    max = 59;
  }
  else if (strcmp(field, "rambyte") == 0) {
    register_to_update = 0x3;
    min = 0;
    max = 255;
  }
  else if (strcmp(field, "offset") == 0) {
    register_to_update = 0x2;
    min = 0;
    max = 63;
  }
  else {
    NRF_LOG_INFO("No such field.\r\n");
    return;
  }

//  err_code = nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &register_to_update, 1, true);
  if (err_code) {
    is_seeed_rtc_connected = false;
  }
  else {
    is_seeed_rtc_connected = true;

    uint8_t intvalue = (uint8_t) atoi(value);
    if (intvalue >= min && intvalue <= max) {
      intvalue = (((intvalue / (uint8_t)10) << (uint8_t)4) | (intvalue % (uint8_t)10)); // convert to BCD
 //     nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &intvalue, 1, false);
    } else {
      NRF_LOG_INFO("Value out of range.\r\n");
    }
  }


#endif // SEEED_RTC
}

void rtc_get_timestamp(timestamp_t stamp) {
  stamp[0] = system_time.clock_time.years_since_y2k;
  stamp[1] = system_time.clock_time.month;
  stamp[2] = system_time.clock_time.day;
  stamp[3] = system_time.clock_time.hours;
  stamp[4] = system_time.clock_time.minutes;
  stamp[5] = system_time.clock_time.seconds;
  stamp[6] = (uint8_t)((system_time.clock_time.millis & 0xFF00) >> 8);
  stamp[7] = (uint8_t)(system_time.clock_time.millis & 0x00FF);
}

void rtc_timestamp_to_timestring(timestamp_t stamp, timestring_t string) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) {
    sprintf(string + 2 * i, "%02X", stamp[i]);
  }
  string[TIMESTRING_SIZE_INCLUDING_NULL - 1] = 0; // terminate with null
}

void rtc_timestring_to_timestamp(timestring_t string, timestamp_t stamp) {
  for(int i = 0 ; i < TIMESTAMP_SIZE ; i++) { // ignore the last (null) character
      stamp[i] = hex_digit_to_uint8(string[2 * i]) << 4; // tens
    stamp[i] += hex_digit_to_uint8(string[2 * i + 1]); // add units
  }
}

bool rtc_is_equal_timestamp(timestamp_t a, timestamp_t b)
{
  bool retval = true;

  for(int i = 0 ; i < 8 ; i++) {
    retval &= (a[i] == b[i]);
  }

  return retval;
}

void rtc_print_status() {
#ifdef SEEED_RTC
  if(is_seeed_rtc_connected) {
    print_seeed_rtc_state();
  }
  else {
    NRF_LOG_INFO("Seeed RTC not responding.\r\n");
  }
#else
  terminal_putstring("No RTC detected.\r\n");
#endif // SEEED_RTC
  rtc_print_date_and_time();
}

void execute_rtc_command(char **parsedCommandArray)
{
  if (strcmp(parsedCommandArray[1], "info") == 0) {
    rtc_print_status();
    return;
  }
  else if (strcmp(parsedCommandArray[1], "set") == 0) {
    rtc_set_field(parsedCommandArray[2], parsedCommandArray[3]);
  }
  else {
    print_help_rtc();
    return;
  }
}

void print_help_rtc() {
  terminal_putstring("rtc info                              Show RTC HW information\r\n");
  terminal_putstring("rtc set <field> <value>               Set RTC\r\n");
  terminal_putstring("\r\n");
  terminal_putstring("    <field> = [ year(0-99), month(1-12), weekday(0-6), day(1-31)\r\n");
  terminal_putstring("                hour(0-23), minute(0-59) or second(0-59) ]\r\n");
  terminal_putstring("\r\n");
}
