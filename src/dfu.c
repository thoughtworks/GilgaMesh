#include <dfu.h>


void restart_in_bootloader_mode()
{
  log("RESTARTING for firmware update...");
  sd_power_gpregret_clr(0xFF);
  EC(sd_power_gpregret_set(BOOTLOADER_DFU_START));
  EC(sd_softdevice_vector_table_base_set(NRF_UICR->BOOTLOADERADDR));
  NVIC_SystemReset();
}


const char *dfu_device_name_with_id() {
  static char hex_chars[] = "0123456789ABCDEF";
  static char device_name[sizeof(DEVICE_NAME_TEMPLATE)] = {0};
  if (device_name[0] == 0) {
    strcpy(device_name, DEVICE_NAME_TEMPLATE);
    uint32_t device_id = NRF_FICR->DEVICEID[1];
    size_t name_length = strlen(device_name);
    for (size_t i = 1; i <= 4; i++) {
      device_name[name_length - i] = hex_chars[device_id % 16];
      device_id /= 16;
    }
  }
  return device_name;
}