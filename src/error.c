#include <ble_gatt.h>
#include <ble_gatts.h>
#include <ble_gattc.h>
#include <ble.h>

#include "system/log.h"
#include "error.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
  UNUSED_PARAMETER(id);
  UNUSED_PARAMETER(pc);

  error_info_t *error_info = (error_info_t *) info;
  if(error_info->err_code != NRF_SUCCESS){
    MESH_LOG("ERROR: ERROR CODE %d, in file %s@%d\r\n",
                   error_info->err_code, error_info->p_file_name, error_info->line_num);
  }
}

const char* getBleEventNameString(uint16_t bleEventId)
{
  switch (bleEventId)
  {
    case BLE_EVT_TX_COMPLETE:
      return "BLE_EVT_TX_COMPLETE";
    case BLE_EVT_USER_MEM_REQUEST:
      return "BLE_EVT_USER_MEM_REQUEST";
    case BLE_EVT_USER_MEM_RELEASE:
      return "BLE_EVT_USER_MEM_RELEASE";
    case BLE_GAP_EVT_CONNECTED:
      return "BLE_GAP_EVT_CONNECTED";
    case BLE_GAP_EVT_DISCONNECTED:
      return "BLE_GAP_EVT_DISCONNECTED";
    case BLE_GAP_EVT_CONN_PARAM_UPDATE:
      return "BLE_GAP_EVT_CONN_PARAM_UPDATE";
    case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
      return "BLE_GAP_EVT_SEC_PARAMS_REQUEST";
    case BLE_GAP_EVT_SEC_INFO_REQUEST:
      return "BLE_GAP_EVT_SEC_INFO_REQUEST";
    case BLE_GAP_EVT_PASSKEY_DISPLAY:
      return "BLE_GAP_EVT_PASSKEY_DISPLAY";
    case BLE_GAP_EVT_AUTH_KEY_REQUEST:
      return "BLE_GAP_EVT_AUTH_KEY_REQUEST";
    case BLE_GAP_EVT_AUTH_STATUS:
      return "BLE_GAP_EVT_AUTH_STATUS";
    case BLE_GAP_EVT_CONN_SEC_UPDATE:
      return "BLE_GAP_EVT_CONN_SEC_UPDATE";
    case BLE_GAP_EVT_TIMEOUT:
      return "BLE_GAP_EVT_TIMEOUT";
    case BLE_GAP_EVT_RSSI_CHANGED:
      return "BLE_GAP_EVT_RSSI_CHANGED";
    case BLE_GAP_EVT_ADV_REPORT:
      return "BLE_GAP_EVT_ADV_REPORT";
    case BLE_GAP_EVT_SEC_REQUEST:
      return "BLE_GAP_EVT_SEC_REQUEST";
    case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
      return "BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST";
    case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
      return "BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP";
    case BLE_GATTC_EVT_REL_DISC_RSP:
      return "BLE_GATTC_EVT_REL_DISC_RSP";
    case BLE_GATTC_EVT_CHAR_DISC_RSP:
      return "BLE_GATTC_EVT_CHAR_DISC_RSP";
    case BLE_GATTC_EVT_DESC_DISC_RSP:
      return "BLE_GATTC_EVT_DESC_DISC_RSP";
    case BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP:
      return "BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP";
    case BLE_GATTC_EVT_READ_RSP:
      return "BLE_GATTC_EVT_READ_RSP";
    case BLE_GATTC_EVT_CHAR_VALS_READ_RSP:
      return "BLE_GATTC_EVT_CHAR_VALS_READ_RSP";
    case BLE_GATTC_EVT_WRITE_RSP:
      return "BLE_GATTC_EVT_WRITE_RSP";
    case BLE_GATTC_EVT_HVX:
      return "BLE_GATTC_EVT_HVX";
    case BLE_GATTC_EVT_TIMEOUT:
      return "BLE_GATTC_EVT_TIMEOUT";
    case BLE_GATTS_EVT_WRITE:
      return "BLE_GATTS_EVT_WRITE";
    case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
      return "BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST";
    case BLE_GATTS_EVT_SYS_ATTR_MISSING:
      return "BLE_GATTS_EVT_SYS_ATTR_MISSING";
    case BLE_GATTS_EVT_HVC:
      return "BLE_GATTS_EVT_HVC";
    case BLE_GATTS_EVT_SC_CONFIRM:
      return "BLE_GATTS_EVT_SC_CONFIRM";
    case BLE_GATTS_EVT_TIMEOUT:
      return "BLE_GATTS_EVT_TIMEOUT";
    default:
      return "Unknown Error";
  }
}