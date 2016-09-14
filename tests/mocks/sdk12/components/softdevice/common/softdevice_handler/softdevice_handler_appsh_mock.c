#include <softdevice_handler_appsh.h>
#include "cmocka_includes.h"

uint32_t softdevice_evt_schedule(void) {
   return mock_type(bool);
}
