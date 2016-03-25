#include "cmocka_includes.h"

// This is a dummy test
static void test_dummy(void **state) {
    (void) state; // unused

    assert_true(true);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dummy),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}