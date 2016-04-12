#pragma once

#include <stdlib.h>
#include <malloc.h>

#include <nrf_soc.h>
#include <softdevice_handler.h>

#include <error.h>
#include <dfu.h>
#include <timer.h>
#include <gap.h>
#include <gatt.h>
#include <connection.h>
#include <logger.h>
#include <terminal.h>
#include <pn532.h>
#include <buzzer.h>

uint32_t initialize();
uint32_t run();
int main(void);
