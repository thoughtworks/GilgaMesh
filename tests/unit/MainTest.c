#include "main.h"
#include "cmocka_includes.h"

static void Main_THIS_IS_A_FAKE_TEST() {
    assert_true(true);
}

int RunMainTest(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(Main_THIS_IS_A_FAKE_TEST),
    };

    return cmocka_run_group_tests_name("MainTest", tests, NULL, NULL);
}

int main() {
    return RunMainTest();
}