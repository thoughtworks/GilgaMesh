#include "cmocka_includes.h"
#include "gatt.h"

// This is a dummy test
static void gattTestOne(void **state) {
    (void) state; // unused

    assert_true(true);
}

static void gattTestTwo(void **state) {
    (void) state; // unused

    assert_false(false);
}

int RunGattTest(void) {
    const struct CMUnitTest tests[] = {
        // list test functions here
        cmocka_unit_test(gattTestOne),
        cmocka_unit_test(gattTestTwo),
    };

    //set test suite name here
    return cmocka_run_group_tests_name("GattTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
    return RunGattTest();
}