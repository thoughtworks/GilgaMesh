#include <stdbool.h>
#include <string.h>

#include <sched.h>
#include <ctype.h>

#include "pn532.h"

#include "cmocka_includes.h"

static void test_id_does_not_exist_in_garbage_data() {
    uint8_t response[] = {'r', 'a', 'n', 'd', 'o', 'm'};
    assert_false(id_exists_in_response(response, sizeof(response)));
}

static void test_id_exists_in_good_response() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2', '3', '4'};
    assert_true(id_exists_in_response(response, sizeof(response)));
}

static void test_id_does_not_exist_in_misleading_data() {
    uint8_t response[] = {'g', 'a', 'r', 'b', 'a', 'g', 'e', '=', 'm', 'i', 's', 'l', 'e', 'a', 'd', 'i', 'n', 'g'};
    assert_false(id_exists_in_response(response, sizeof(response)));
}

static void test_id_does_not_exist_in_data_that_is_too_small() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2'};
    assert_false(id_exists_in_response(response, sizeof(response)));
}

static void test_get_id() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2', '3', '4'};
    assert_true(1234 == get_id(response, sizeof(response)));
}

int RunPn532Test(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_id_exists_in_good_response),
            cmocka_unit_test(test_id_does_not_exist_in_garbage_data),
            cmocka_unit_test(test_id_does_not_exist_in_misleading_data),
            cmocka_unit_test(test_id_does_not_exist_in_data_that_is_too_small),
            cmocka_unit_test(test_get_id),
    };

    return cmocka_run_group_tests_name("Pn532Test", tests, NULL, NULL);
}

int main() {
    RunPn532Test();
}
