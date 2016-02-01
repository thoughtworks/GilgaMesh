#include <stdlib.h>
#include <nrf_soc.h>
#include <app_error.h>
#include <softdevice_handler.h>
#include <app_timer.h>
#include <malloc.h>
#include <logger.h>
#include <led.h>
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

	//Start Timers
	//***** initTimers();


	while (true)
	{
		uint32_t err = NRF_ERROR_NOT_FOUND;
		//***** Terminal::PollUART();

		do {

			sizeOfCurrentEvent = sizeOfEvent;
			err = sd_ble_evt_get(currentEventBuffer, &sizeOfCurrentEvent);

			if (err == NRF_SUCCESS)
			{
				log("EVENT: --- EVENT_HANDLER %d -----", currentEvent->header.evt_id);
				bleDispatchEventHandler(currentEvent);
			}

			else if (err == NRF_ERROR_NOT_FOUND) {
				//if (node && node->passsedTimeSinceLastTimerHandler > 0) {
					//node->TimerTickHandler(node->passsedTimeSinceLastTimerHandler);
					//timerEventDispatch(node->passsedTimeSinceLastTimerHandler, node->appTimerMs);
					//node->passsedTimeSinceLastTimerHandler = 0;
				//}

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


//Dispatches system events
void sys_evt_dispatch(uint32_t sys_evt)
{
	//Hand system events to the pstorage library
    pstorage_sys_event_handler(sys_evt);

    //Dispatch system events to all modules
	//for(int i=0; i<MAX_MODULE_COUNT; i++){
		//if(node != NULL && node->activeModules[i] != NULL && node->activeModules[i]->configurationPointer->moduleActive){
			//node->activeModules[i]->SystemEventHandler(sys_evt);
		//}
	//}

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


//The app_error handler is called by all APP_ERROR_CHECK functions
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
	//We want to debug DEADBEEF => Endless loop.
	if(error_code == 0xDEADBEEF){
    NVIC_SystemReset();
		while(1){
		}
	}

	//Output Error message to UART
	if(error_code != NRF_SUCCESS){
		const char* error_string = getNrfErrorString(error_code);
		log("ERROR: ERROR CODE %d: %s, in file %s@%d", error_code, error_string, p_file_name, line_num);
	}

	//Invalid states are bad and should be debugged, but should not necessarily
	//Break the program every time they happen.
	//FIXME: must not ever happen, so fix that
	if (error_code == NRF_ERROR_INVALID_STATE){
     NVIC_SystemReset();
		return;
	}

	//NRF_ERROR_BUSY is not an error(tm)
	//FIXME: above statement is not true
	if (error_code == NRF_ERROR_BUSY) {
    NVIC_SystemReset();
		return;
	}

    NVIC_SystemReset();
}

//Called when the softdevice crashes
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
	//Does not produce interesting filename,....
    app_error_handler(0xDEADBEEF, 0, NULL);
}

//This is, where the program will get stuck in the case of a Hard fault
void HardFault_Handler(void)
{
	for (;;)
	{
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

/********** C++ stuff, haven't converted yet

//### TIMERS ##############################################################
static app_timer_id_t mainTimerMsId; // Main timer

//Called by the app_timer module
static void ble_timer_dispatch(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    //We just increase the time that has passed since the last handler
    //And call the timer from our main event handling queue
    node->passsedTimeSinceLastTimerHandler += Config->mainTimerTickMs;

    //Timer handlers are called from the main event handling queue and from timerEventDispatch
}

//This function is called from the main event handling
static void timerEventDispatch(u16 passedTime, u32 appTimer){
	//Dispatch event to all modules
	for(int i=0; i<MAX_MODULE_COUNT; i++){
		if(node != NULL && node->activeModules[i] != 0  && node->activeModules[i]->configurationPointer->moduleActive){
			node->activeModules[i]->TimerEventHandler(passedTime, appTimer);
		}
	}
}

//Starts an application timer
void initTimers(void){
	u32 err = 0;

	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);

	err = app_timer_create(&mainTimerMsId, APP_TIMER_MODE_REPEATED, ble_timer_dispatch);
    APP_ERROR_CHECK(err);

	err = app_timer_start(mainTimerMsId, APP_TIMER_TICKS(Config->mainTimerTickMs, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err);
}

*/

/**
 *@}
 **/
