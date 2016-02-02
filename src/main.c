#include <main.h>

static uint8_t currentEventBuffer[sizeof(ble_evt_t) + BLE_L2CAP_MTU_DEF];
static ble_evt_t* currentEvent = (ble_evt_t *) currentEventBuffer;
static const uint16_t sizeOfEvent = sizeof(currentEventBuffer);
static uint16_t sizeOfCurrentEvent = sizeof(currentEventBuffer);

//***** Node* node = NULL;

bool lookingForInvalidStateErrors = false;

//***** Conf* Conf::instance;



int main(void)
{
    uint32_t err;

    //***** Terminal::Init();
    bleInit();
    //***** Storage::getInstance();
    //***** node = new Node(Config->meshNetworkIdentifier);

    //***** new Testing();

    timer_initialize();


    while (true)
    {
        uint32_t err = NRF_ERROR_NOT_FOUND;
        //***** Terminal::PollUART();

        do {
            sizeOfCurrentEvent = sizeOfEvent;
            err = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

            if (err == NRF_SUCCESS) {
                log("EVENT: --- EVENT_HANDLER %d -----", currentEvent->header.evt_id);
                bleDispatchEventHandler(currentEvent);
            }

            else if (err == NRF_ERROR_NOT_FOUND) {
                timer_tick_handler();

                err = sd_app_evt_wait();
                APP_ERROR_CHECK(err);
                sd_nvic_ClearPendingIRQ(SD_EVT_IRQn);
                break;

            } else {
                APP_ERROR_CHECK(err);
                break;
            }
        } while (true);
    }
}

void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
}


void bleInit(void){
    uint32_t err = 0;

    log("NODE: Initializing Softdevice");

    err = softdevice_handler_init(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, currentEventBuffer, sizeOfEvent, NULL);
    APP_ERROR_CHECK(err);

    err = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err);

    ble_enable_params_t bleSdEnableParams;
    memset(&bleSdEnableParams, 0, sizeof(bleSdEnableParams));
    bleSdEnableParams.gatts_enable_params.attr_tab_size = ATTR_TABLE_MAX_SIZE;
    bleSdEnableParams.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

    err = sd_ble_enable(&bleSdEnableParams);
    APP_ERROR_CHECK(err);

    err = sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
    APP_ERROR_CHECK(err);

    err = sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    APP_ERROR_CHECK(err);
}


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    //We want to debug DEADBEEF => Endless loop.
    if(error_code == 0xDEADBEEF){
        NVIC_SystemReset();
        while(1){
        }
    }

    if(error_code != NRF_SUCCESS){
        const char* error_string = getNrfErrorString(error_code);
        log("ERROR: ERROR CODE %d: %s, in file %s@%d", error_code, error_string, p_file_name, line_num);
    }

    if (error_code == NRF_ERROR_INVALID_STATE){
        NVIC_SystemReset();
        return;
    }

    if (error_code == NRF_ERROR_BUSY) {
        NVIC_SystemReset();
        return;
    }

    NVIC_SystemReset();
}


void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, 0, NULL);
}

void HardFault_Handler(void)
{
    for (;;){
        // Endless debugger loop
    }
}

void bleDispatchEventHandler(ble_evt_t * bleEvent)
{
    uint16_t eventId = bleEvent->header.evt_id;
    log("EVENTS: BLE EVENT %s (%d)", getBleEventNameString(eventId), eventId);

    //Call an event handler of some kind...

    log("EVENTS: End of event");
}
