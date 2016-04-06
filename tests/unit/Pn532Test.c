#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <sched.h>
#include <ctype.h>

#include "app_util_platform.h"

bool id_exists_in_response(uint8_t *response, size_t response_length) {
    char *first_occurrence_of_equals = memchr(response, '=', response_length);

    if (!first_occurrence_of_equals) return false;

    int expected_digits_in_id = 4;
    for (int i = 1; i <= expected_digits_in_id; i++) {
        if (!isdigit(first_occurrence_of_equals[i])) return false;
    }

    return true;
}

void test_id_does_not_exist_in_garbage_data() {
    uint8_t response[] = {'r', 'a', 'n', 'd', 'o', 'm'};
    assert(!id_exists_in_response(response, sizeof(response)));
}

void test_id_exists_in_good_response() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2', '3', '4'};
    assert(id_exists_in_response(response, sizeof(response)));
}

void test_id_does_not_exist_in_misleading_data() {
    uint8_t response[] = {'g', 'a', 'r', 'b', 'a', 'g', 'e', '=', 'm', 'i', 's', 'l', 'e', 'a', 'd', 'i', 'n', 'g'};
    assert(!id_exists_in_response(response, sizeof(response)));
}

void test_id_does_not_exist_in_data_that_is_too_small() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2'};
    assert(!id_exists_in_response(response, sizeof(response)));
}




short get_id(uint8_t *response, size_t response_length) {
    char *first_occurrence_of_equals = memchr(response, '=', response_length);

    int first_digit = first_occurrence_of_equals[1] - '0';
    int second_digit = first_occurrence_of_equals[2] - '0';
    int third_digit = first_occurrence_of_equals[3] - '0';
    int fourth_digit = first_occurrence_of_equals[4] - '0';

    short full_id = (short) (first_digit * 1000 + second_digit * 100 + third_digit * 10 + fourth_digit);

    return full_id;
}

void test_get_id() {
    uint8_t response[] = {'q', 'c', 'o', 'n', 's', 'f', '.', 'c', 'o', 'm', '/', '?', 'i', 'd', '=', '1', '2', '3', '4'};
    assert(1234 == get_id(response, sizeof(response)));
}

int main() {
    test_id_exists_in_good_response();
    test_id_does_not_exist_in_garbage_data();
    test_id_does_not_exist_in_misleading_data();
    test_id_does_not_exist_in_data_that_is_too_small();
    test_get_id();
}
