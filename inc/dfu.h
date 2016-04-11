#pragma once

#include <stdlib.h>
#include <string.h>

#include <nrf_sdm.h>
#include <bootloader_util.h>
#include <bootloader_types.h>

#include <error.h>

#define DEVICE_NAME_TEMPLATE             "VB-0000"
#define IS_SRVC_CHANGED_CHARACT_PRESENT  1


void restart_in_bootloader_mode(void);
const char *dfu_device_name_with_id(void);
