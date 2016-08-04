#include "system/storage.h"
#include <pstorage.h>

sys_handle_storage_event(uint32_t sys_evt) {
  pstorage_sys_event_handler(sys_evt);
}
