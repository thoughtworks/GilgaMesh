#include <rtc.h>
#include <stdlib.h>
#include "conversion.h"
#include "logger.h"
#include "terminal.h"

#ifdef SEEED_RTC
#include <nrf_drv_twi.h>
#include <rtc.h>
#include <sdk_common.h>
#include <app_scheduler.h>

static const nrf_drv_twi_t rtcInterface = NRF_DRV_TWI_INSTANCE(0);
static const nrf_drv_twi_config_t rtcConfig = NRF_DRV_TWI_DEFAULT_CONFIG(0);
static const uint8_t seeed_rtc_address = 81;
static bool is_seeed_rtc_connected = false;
static PCF85063TPState_t rtc_state;

static void twi_event_handler(nrf_drv_twi_evt_t *twiEvent) {
  if(twiEvent->type == NRF_DRV_TWI_TX_DONE) {
    log("I2C: Sent LEN: %d DATA: %d", twiEvent->length, *(twiEvent->p_data));
  }
  else if(twiEvent->type == NRF_DRV_TWI_RX_DONE) {
    log("I2C: Received LEN: %d DATA: %d", twiEvent->length, *(twiEvent->p_data));
  }
  else if(twiEvent->type == NRF_DRV_TWI_ERROR) {
    if(twiEvent->error_src == NRF_TWI_ERROR_ADDRESS_NACK) {
      log("I2C: Address error");
    }
    else if(twiEvent->error_src == NRF_TWI_ERROR_OVERRUN_NACK) {
      log("I2C: Data overrun error");
    }
    else if(twiEvent->error_src == NRF_TWI_ERROR_DATA_NACK) {
      log("I2C: Data NACK error");
    }
    else {
      log("I2C: Unknown error");
    }
  }
}

static void read_seeed_rtc_state() {
  uint32_t err_code;
  uint8_t rtc_register = 0x0;

  err_code = nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
  if(!err_code) {
    is_seeed_rtc_connected = true;
    memset(&rtc_state, 0, sizeof(rtc_state));

    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_0.Control_1, 1, false);

    rtc_register = 0x1;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_1.Control_2, 1, false);

    rtc_register = 0x2;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_2.Offset, 1, false);

    rtc_register = 0x3;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_3.RAM_byte, 1, false);

    rtc_register = 0x4;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_4.Seconds, 1, false);

    rtc_register = 0x5;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_5.Minutes, 1, false);

    rtc_register = 0x6;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_6.Hours, 1, false);

    rtc_register = 0x7;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_7.Days, 1, false);

    rtc_register = 0x8;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_8.Weekdays, 1, false);

    rtc_register = 0x9;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_9.Months, 1, false);

    rtc_register = 0xA;
    nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &rtc_register, 1, false);
    nrf_drv_twi_rx(&rtcInterface, seeed_rtc_address, &rtc_state.register_A.Years, 1, false);
  }
  else {
    is_seeed_rtc_connected = false;
  }

}

static void print_seeed_rtc_state() {
  read_seeed_rtc_state();

  if(is_seeed_rtc_connected) {
    log("RTC Control_1 CS:%d 12H:%d CIE:%d SR:%d STOP:%d TEST:%d", rtc_state.register_0.CAP_SEL,
        rtc_state.register_0.AM_PM,
        rtc_state.register_0.CIE,
        rtc_state.register_0.SR,
        rtc_state.register_0.STOP,
        rtc_state.register_0.EXT_TEST);
    log("RTC Control_2 COF:%d TF:%d HMI:%d MI:%d", rtc_state.register_1.COF,
        rtc_state.register_1.TF,
        rtc_state.register_1.HMI,
        rtc_state.register_1.MI);
    log("RTC Offset: %d", rtc_state.register_2.Offset);
    log("RTC RAM_byte: %d", rtc_state.register_3.RAM_byte);
    log("RTC Seconds OS:%d SECONDS:%d%d", rtc_state.register_4.OS,
        rtc_state.register_4.TENS,
        rtc_state.register_4.UNIT);
    log("RTC Minutes: %d%d", rtc_state.register_5.TENS,
        rtc_state.register_5.UNIT);
    log("RTC Hours: %d%d", rtc_state.register_6.TENS,
        rtc_state.register_6.UNIT);
    log("RTC Days: %d%d", rtc_state.register_7.TENS,
        rtc_state.register_7.UNIT);
    log("RTC Weekdays: %d", rtc_state.register_8.UNIT);
    log("RTC Months: %d%d", rtc_state.register_9.TENS,
        rtc_state.register_9.UNIT);
    log("RTC Years: 20%d%d", rtc_state.register_A.TENS,
        rtc_state.register_A.UNIT);
  }
};

static void seeed_rtc_init() {
  uint32_t err_code;
  log("Initializing Seeed RTC");

  err_code = nrf_drv_twi_init(&rtcInterface, &rtcConfig, NULL);
  APP_ERROR_CHECK(err_code);
  nrf_drv_twi_enable(&rtcInterface);

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

void rtc_init() {
  system_time.clock_time.day = 1;
  system_time.clock_time.month = 1;
  system_time.clock_time.year = 2000;

#ifdef SEEED_RTC
  seeed_rtc_init();
#endif // SEEED_RTC
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
  log("System time is: %d/%d/%d %02d:%02d:%02d.%03d Offset: %d minutes to UTC", system_time.clock_time.month
                                                                              , system_time.clock_time.day
                                                                              , system_time.clock_time.year
                                                                              , system_time.clock_time.hours
                                                                              , system_time.clock_time.minutes
                                                                              , system_time.clock_time.seconds
                                                                              , system_time.clock_time.millis
                                                                              , system_time.clock_time.UTC_offset_in_minutes);

  timestamp_t timestamp;
  timestring_t timestring;
  rtc_get_timestamp(timestamp);
  rtc_timestamp_to_timestring(timestamp, timestring);
  log("Timestamp: %s", timestring);
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
    log("No such field.");
    return;
  }

  err_code = nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &register_to_update, 1, true);
  if (err_code) {
    is_seeed_rtc_connected = false;
  }
  else {
    is_seeed_rtc_connected = true;

    uint8_t intvalue = (uint8_t) atoi(value);
    if (intvalue >= min && intvalue <= max) {
      intvalue = (((intvalue / (uint8_t)10) << (uint8_t)4) | (intvalue % (uint8_t)10)); // convert to BCD
      nrf_drv_twi_tx(&rtcInterface, seeed_rtc_address, &intvalue, 1, false);
    } else {
      log("Value out of range.");
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


void rtc_print_status() {
#ifdef SEEED_RTC
  if(is_seeed_rtc_connected) {
    print_seeed_rtc_state();
  }
  else {
    log("Seeed RTC not responding.");
  }
#else
  terminal_putstring("No RTC detected.");
#endif // SEEED_RTC
  rtc_print_date_and_time();
}
