#include "cmocka_includes.h"

// This is a dummy test
static void dummyTestOne(void **state) {
    (void) state; // unused

    assert_true(true);
}

static void dummyTestTwo(void **state) {
    (void) state; // unused

    assert_false(false);
}

static void StopAdvertizingTest(void **state) {
    (void) state; // unused

    //stop_advertising();
}

int RunGapTest(void) {
    const struct CMUnitTest tests[] = {
        // list test functions here
        cmocka_unit_test(dummyTestOne),
        cmocka_unit_test(dummyTestTwo),
        cmocka_unit_test(StopAdvertizingTest),
    };

    //set test suite name here
    return cmocka_run_group_tests_name("GapTest", tests, NULL, NULL);
}