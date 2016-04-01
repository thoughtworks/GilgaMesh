#include "cmocka_includes.h"
#include "gap.h"
#include "led.h"

static void StopAdvertizingTest(void **state) {
    (void) state; // unused

    will_return(sd_ble_gap_adv_stop, 0); // setup mock to return error code 0
    stop_advertising(); // exercise the SUT
}

int RunGapTest(void) {
    const struct CMUnitTest tests[] = {
        // list test functions here
        cmocka_unit_test(StopAdvertizingTest),
    };

    //set test suite name here
    return cmocka_run_group_tests_name("GapTest", tests, NULL, NULL);
}