#include "cmocka_includes.h"

void pstorage_sys_event_handler (uint32_t sys_evt) {
   check_expected(sys_evt);
}