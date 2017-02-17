#include "softdevice.h"
#include <ble_conn_state.h>
#include <fstorage.h>
#include <nrf52/nrf_mbr.h>
#include <nrf_sdm.h>
#include <nrf_log.h>
#include <softdevice_handler_appsh.h>
#include "system/log.h"
#include "boards.h"
#include "connection.h"
#include "error.h"
#include "gap.h"

void softdevice_fault_callback(uint32_t id, uint32_t pc, uint32_t info) {
  app_error_fault_handler(id, pc, info); // delegate to error module
}

static void ble_evt_dispatch(ble_evt_t * p_ble_evt) {
  ble_conn_state_on_ble_evt(p_ble_evt);
  handle_gap_event(p_ble_evt); // delegate to gap module
}

static void sys_evt_dispatch(uint32_t sys_evt) {
  fs_sys_event_handler(sys_evt);
}

bool softdevice_initialize() {
  sd_mbr_command_t com = {SD_MBR_COMMAND_INIT_SD, };
  nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

  // Initialize SoftDevice.
  MESH_LOG_DEBUG("  * mbr initialization\r\n");
  if (!execute_succeeds(sd_mbr_command(&com))) return false;

  MESH_LOG_DEBUG("  * vector table\r\n");
  if (!execute_succeeds(sd_softdevice_vector_table_base_set(BOOTLOADER_REGION_START))) return false;

  // Initialize the SoftDevice handler module.
  MESH_LOG_DEBUG("  * softdevice handler\r\n");
  SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);

  ble_enable_params_t ble_enable_params;
  MESH_LOG_DEBUG("  * BLE params: maximum of %d central connections and %d peripheral connections\r\n",
                 MAX_CENTRAL_CONNECTIONS,
                 MAX_PERIPHERAL_CONNECTIONS);
  if (!execute_succeeds(softdevice_enable_get_default_config(MAX_PERIPHERAL_CONNECTIONS,
                                                            MAX_CENTRAL_CONNECTIONS,
                                                            &ble_enable_params))) return false;

  // Enable BLE stack.
  MESH_LOG_DEBUG("  * enable softdevice\r\n");
  if (!execute_succeeds(softdevice_enable(&ble_enable_params))) return false;

  // Register with the SoftDevice handler module for BLE events.
  MESH_LOG_DEBUG("  * register BLE event handler\r\n");
  if (!execute_succeeds(softdevice_ble_evt_handler_set(ble_evt_dispatch))) return false;

  // Register with the SoftDevice handler module for system events.
  MESH_LOG_DEBUG("  * register system event handler\r\n");
  if (!execute_succeeds(softdevice_sys_evt_handler_set(sys_evt_dispatch))) return false;

  return true;
}

