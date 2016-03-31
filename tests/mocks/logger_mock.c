#include <logger.h>
#include <ble.h>
#include <ble_hci.h>
#include <pstorage.h>
#include <stdarg.h>

#include "cmocka_includes.h"

char mhTraceBuffer[TRACE_BUFFER_SIZE] = { 0 };

log_f(bool printLine, const char* file, int32_t line, const char* message, ...)
{
   memset(mhTraceBuffer, 0, TRACE_BUFFER_SIZE);

   //Variable argument list must be passed to vnsprintf
   va_list aptr;
   va_start(aptr, message);
   vsnprintf(mhTraceBuffer, TRACE_BUFFER_SIZE, message, aptr);
   va_end(aptr);

   snprintf(mhTraceBuffer, TRACE_BUFFER_SIZE, "[%s@%d]: %s\r\n", file, line, mhTraceBuffer);

}


/*################### Error codes #########################*/
const char* getNrfErrorString(uint32_t nrfErrorCode)
{
   switch (nrfErrorCode)
   {
      case NRF_SUCCESS:
         return "NRF_SUCCESS";
      case NRF_ERROR_SVC_HANDLER_MISSING:
         return "NRF_ERROR_SVC_HANDLER_MISSING";
      case NRF_ERROR_SOFTDEVICE_NOT_ENABLED:
         return "NRF_ERROR_SOFTDEVICE_NOT_ENABLED";
      case NRF_ERROR_INTERNAL:
         return "NRF_ERROR_INTERNAL";
      case NRF_ERROR_NO_MEM:
         return "NRF_ERROR_NO_MEM";
      case NRF_ERROR_NOT_FOUND:
         return "NRF_ERROR_NOT_FOUND";
      case NRF_ERROR_NOT_SUPPORTED:
         return "NRF_ERROR_NOT_SUPPORTED";
      case NRF_ERROR_INVALID_PARAM:
         return "NRF_ERROR_INVALID_PARAM";
      case NRF_ERROR_INVALID_STATE:
         return "NRF_ERROR_INVALID_STATE";
      case NRF_ERROR_INVALID_LENGTH:
         return "NRF_ERROR_INVALID_LENGTH";
      case NRF_ERROR_INVALID_FLAGS:
         return "NRF_ERROR_INVALID_FLAGS";
      case NRF_ERROR_INVALID_DATA:
         return "NRF_ERROR_INVALID_DATA";
      case NRF_ERROR_DATA_SIZE:
         return "NRF_ERROR_DATA_SIZE";
      case NRF_ERROR_TIMEOUT:
         return "NRF_ERROR_TIMEOUT";
      case NRF_ERROR_NULL:
         return "NRF_ERROR_NULL";
      case NRF_ERROR_FORBIDDEN:
         return "NRF_ERROR_FORBIDDEN";
      case NRF_ERROR_INVALID_ADDR:
         return "NRF_ERROR_INVALID_ADDR";
      case NRF_ERROR_BUSY:
         return "NRF_ERROR_BUSY";
      case BLE_ERROR_INVALID_CONN_HANDLE:
         return "BLE_ERROR_INVALID_CONN_HANDLE";
      case BLE_ERROR_INVALID_ATTR_HANDLE:
         return "BLE_ERROR_INVALID_ATTR_HANDLE";
      case BLE_ERROR_NO_TX_BUFFERS:
         return "BLE_ERROR_NO_TX_BUFFERS";
      case 0xDEADBEEF:
         return "DEADBEEF";
      default:
         return "UNKNOWN_ERROR";
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

const char* getHciErrorString(uint8_t hciErrorCode)
{
   switch (hciErrorCode)
   {

      case BLE_HCI_STATUS_CODE_SUCCESS:
         return "Success";
      case BLE_HCI_STATUS_CODE_UNKNOWN_BTLE_COMMAND:
         return "Unknown BLE Command";
      case BLE_HCI_STATUS_CODE_UNKNOWN_CONNECTION_IDENTIFIER:
         return "Unknown Connection Identifier";
      case BLE_HCI_AUTHENTICATION_FAILURE:
         return "Authentication Failure";
      case BLE_HCI_CONN_FAILED_TO_BE_ESTABLISHED:
         return "Connection Failed to be Established";
      case BLE_HCI_CONN_INTERVAL_UNACCEPTABLE:
         return "Connection Interval Unacceptable";
      case BLE_HCI_CONN_TERMINATED_DUE_TO_MIC_FAILURE:
         return "Connection Terminated due to MIC Failure";
      case BLE_HCI_CONNECTION_TIMEOUT:
         return "Connection Timeout";
      case BLE_HCI_CONTROLLER_BUSY:
         return "Controller Busy";
      case BLE_HCI_DIFFERENT_TRANSACTION_COLLISION:
         return "Different Transaction Collision";
      case BLE_HCI_DIRECTED_ADVERTISER_TIMEOUT:
         return "Directed Adverisement Timeout";
      case BLE_HCI_INSTANT_PASSED:
         return "Instant Passed";
      case BLE_HCI_LOCAL_HOST_TERMINATED_CONNECTION:
         return "Local Host Terminated Connection";
      case BLE_HCI_MEMORY_CAPACITY_EXCEEDED:
         return "Memory Capacity Exceeded";
      case BLE_HCI_PAIRING_WITH_UNIT_KEY_UNSUPPORTED:
         return "Pairing with Unit Key Unsupported";
      case BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES:
         return "Remote Device Terminated Connection due to low resources";
      case BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF:
         return "Remote Device Terminated Connection due to power off";
      case BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION:
         return "Remote User Terminated Connection";
      case BLE_HCI_STATUS_CODE_COMMAND_DISALLOWED:
         return "Command Disallowed";
      case BLE_HCI_STATUS_CODE_INVALID_BTLE_COMMAND_PARAMETERS:
         return "Invalid BLE Command Parameters";
      case BLE_HCI_STATUS_CODE_INVALID_LMP_PARAMETERS:
         return "Invalid LMP Parameters";
      case BLE_HCI_STATUS_CODE_LMP_PDU_NOT_ALLOWED:
         return "LMP PDU Not Allowed";
      case BLE_HCI_STATUS_CODE_LMP_RESPONSE_TIMEOUT:
         return "LMP Response Timeout";
      case BLE_HCI_STATUS_CODE_PIN_OR_KEY_MISSING:
         return "Pin or Key missing";
      case BLE_HCI_STATUS_CODE_UNSPECIFIED_ERROR:
         return "Unspecified Error";
      case BLE_HCI_UNSUPPORTED_REMOTE_FEATURE:
         return "Unsupported Remote Feature";
      default:
         return "Unknown HCI error";
   }
}

const char* getGattStatusErrorString(uint16_t gattStatusCode)
{
   switch (gattStatusCode)
   {
      case BLE_GATT_STATUS_SUCCESS:
         return "Success";
      case BLE_GATT_STATUS_UNKNOWN:
         return "Unknown or not applicable status";
      case BLE_GATT_STATUS_ATTERR_INVALID:
         return "ATT Error: Invalid Error Code";
      case BLE_GATT_STATUS_ATTERR_INVALID_HANDLE:
         return "ATT Error: Invalid Attribute Handle";
      case BLE_GATT_STATUS_ATTERR_READ_NOT_PERMITTED:
         return "ATT Error: Read not permitted";
      case BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED:
         return "ATT Error: Write not permitted";
      case BLE_GATT_STATUS_ATTERR_INVALID_PDU:
         return "ATT Error: Used in ATT as Invalid PDU";
      case BLE_GATT_STATUS_ATTERR_INSUF_AUTHENTICATION:
         return "ATT Error: Authenticated link required";
      case BLE_GATT_STATUS_ATTERR_REQUEST_NOT_SUPPORTED:
         return "ATT Error: Used in ATT as Request Not Supported";
      case BLE_GATT_STATUS_ATTERR_INVALID_OFFSET:
         return "ATT Error: Offset specified was past the end of the attribute";
      case BLE_GATT_STATUS_ATTERR_INSUF_AUTHORIZATION:
         return "ATT Error: Used in ATT as Insufficient Authorisation";
      case BLE_GATT_STATUS_ATTERR_PREPARE_QUEUE_FULL:
         return "ATT Error: Used in ATT as Prepare Queue Full";
      case BLE_GATT_STATUS_ATTERR_ATTRIBUTE_NOT_FOUND:
         return "ATT Error: Used in ATT as Attribute not found";
      case BLE_GATT_STATUS_ATTERR_ATTRIBUTE_NOT_LONG:
         return "ATT Error: Attribute cannot be read or written using read/write blob requests";
      case BLE_GATT_STATUS_ATTERR_INSUF_ENC_KEY_SIZE:
         return "ATT Error: Encryption key size used is insufficient";
      case BLE_GATT_STATUS_ATTERR_INVALID_ATT_VAL_LENGTH:
         return "ATT Error: Invalid value size";
      case BLE_GATT_STATUS_ATTERR_UNLIKELY_ERROR:
         return "ATT Error: Very unlikely error";
      case BLE_GATT_STATUS_ATTERR_INSUF_ENCRYPTION:
         return "ATT Error: Encrypted link required";
      case BLE_GATT_STATUS_ATTERR_UNSUPPORTED_GROUP_TYPE:
         return "ATT Error: Attribute type is not a supported grouping attribute";
      case BLE_GATT_STATUS_ATTERR_INSUF_RESOURCES:
         return "ATT Error: Encrypted link required";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE1_BEGIN:
         return "ATT Error: Reserved for Future Use range #1 begin";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE1_END:
         return "ATT Error: Reserved for Future Use range #1 end";
      case BLE_GATT_STATUS_ATTERR_APP_BEGIN:
         return "ATT Error: Application range begin";
      case BLE_GATT_STATUS_ATTERR_APP_END:
         return "ATT Error: Application range end";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE2_BEGIN:
         return "ATT Error: Reserved for Future Use range #2 begin";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE2_END:
         return "ATT Error: Reserved for Future Use range #2 end";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE3_BEGIN:
         return "ATT Error: Reserved for Future Use range #3 begin";
      case BLE_GATT_STATUS_ATTERR_RFU_RANGE3_END:
         return "ATT Error: Reserved for Future Use range #3 end";
      case BLE_GATT_STATUS_ATTERR_CPS_CCCD_CONFIG_ERROR:
         return "ATT Common Profile and Service Error: Client Characteristic Configuration Descriptor improperly configured";
      case BLE_GATT_STATUS_ATTERR_CPS_PROC_ALR_IN_PROG:
         return "ATT Common Profile and Service Error: Procedure Already in Progress";
      case BLE_GATT_STATUS_ATTERR_CPS_OUT_OF_RANGE:
         return "ATT Common Profile and Service Error: Out Of Range";
      default:
         return "Unknown GATT status";
   }
}


const char* getPstorageStatusErrorString(uint16_t operationCode)
{
   switch(operationCode){
      case PSTORAGE_CLEAR_OP_CODE:
         return "Error when Clear Operation was requested";
      case PSTORAGE_LOAD_OP_CODE:
         return "Error when Load Operation was requested";
      case PSTORAGE_STORE_OP_CODE:
         return "Error when Store Operation was requested";
      case PSTORAGE_UPDATE_OP_CODE:
         return "Update an already touched storage block";
      default:
         return "Unknown operation code";
   }
}

