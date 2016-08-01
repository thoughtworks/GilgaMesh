#include "cmocka_includes.h"
#include "gap.h"

static void StartScanningTest(void **state) {
    (void) state; // unused

    will_return(sd_ble_gap_scan_start, 0); // setup mock to return error code 0
    start_scanning(); // exercise the SUT
}

int RunGapTest(void) {
    const struct CMUnitTest tests[] = {
        // list test functions here
        cmocka_unit_test(StartScanningTest),
    };

    //set test suite name here
    return cmocka_run_group_tests_name("GapTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
   return RunGapTest();
}