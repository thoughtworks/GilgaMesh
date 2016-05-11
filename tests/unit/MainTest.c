#include "cmocka_includes.h"
#include "main.h"

// This is a dummy test
static void mainTestOne(void **state) {
    (void) state; // unused

    assert_true(true);
}

static void mainTestTwo(void **state) {
    (void) state; // unused

    assert_false(false);
}

int RunMainTest(void) {
    const struct CMUnitTest tests[] = {
        // list test functions here
        cmocka_unit_test(mainTestOne),
        cmocka_unit_test(mainTestTwo),
    };

    //set test suite name here
    return cmocka_run_group_tests_name("MainTest", tests, NULL, NULL);
}

int main(int argc, char **argv) {
    return RunMainTest();
}