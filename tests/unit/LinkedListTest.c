#include <linked_list.h>

#include "cmocka_includes.h"


static linkedList list;
static void initialize_list_with_max_size(uint16_t size) {
  memset(&list, 0, sizeof(list));
  list.maxSize = size;
}


static void test_add_item_returns_false_when_list_becomes_full() {
  initialize_list_with_max_size(1);

  assert_true(add_item_to_list(&list, "foo"));
  assert_false(add_item_to_list(&list, "bar"));
}

static void test_add_item_increments_list_size() {
  initialize_list_with_max_size(5);
  assert_int_equal(0, list.count);

  add_item_to_list(&list, "foo");
  assert_int_equal(1, list.count);
}

static void test_add_item_appends_to_end_of_list() {
  initialize_list_with_max_size(5);

  add_item_to_list(&list, "foo");
  assert_string_equal("foo", list.first->item);
  assert_string_equal("foo", list.last->item);

  add_item_to_list(&list, "bar");
  assert_string_equal("foo", list.first->item);
  assert_string_equal("bar", list.last->item);

  add_item_to_list(&list, "baz");
  assert_string_equal("foo", list.first->item);
  assert_string_equal("baz", list.last->item);

}

static void test_remove_item_returns_false_when_list_is_empty() {
  initialize_list_with_max_size(5);

  char *item = "foo";
  assert_false(remove_item_from_list(&list, item, 4));
}

static void test_remove_item_returns_false_when_item_is_not_in_list() {
  initialize_list_with_max_size(5);
  add_item_to_list(&list, "foo");

  char *item = "bar";
  assert_false(remove_item_from_list(&list, item, 4));
}

static void test_remove_item_returns_true_when_item_is_in_list() {
  initialize_list_with_max_size(5);
  add_item_to_list(&list, "foo");

  char *item = "foo";
  assert_true(remove_item_from_list(&list, item, 4));
}

static void test_remove_item_decrements_list_size() {
  initialize_list_with_max_size(5);
  add_item_to_list(&list, "foo");
  assert_int_equal(1, list.count);

  remove_item_from_list(&list, "foo", 4);
  assert_int_equal(0, list.count);
}

static void test_remove_first_item_from_list() {
  initialize_list_with_max_size(5);
  add_item_to_list(&list, "foo");
  add_item_to_list(&list, "bar");

  remove_item_from_list(&list, "foo", 4);
  assert_string_equal("bar", list.first->item);
}

static void test_remove_last_item_from_list() {
  initialize_list_with_max_size(5);
  add_item_to_list(&list, "foo");
  add_item_to_list(&list, "bar");
  add_item_to_list(&list, "baz");

  remove_item_from_list(&list, "baz", 4);
  assert_string_equal("bar", list.last->item);
}

int RunLinkedListTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(test_add_item_returns_false_when_list_becomes_full),
          cmocka_unit_test(test_add_item_increments_list_size),
          cmocka_unit_test(test_add_item_appends_to_end_of_list),
          cmocka_unit_test(test_remove_item_returns_false_when_list_is_empty),
          cmocka_unit_test(test_remove_item_returns_false_when_item_is_not_in_list),
          cmocka_unit_test(test_remove_item_returns_true_when_item_is_in_list),
          cmocka_unit_test(test_remove_item_decrements_list_size),
          cmocka_unit_test(test_remove_first_item_from_list),
          cmocka_unit_test(test_remove_last_item_from_list),
  };

  return cmocka_run_group_tests_name("LinkedListTest", tests, NULL, NULL);
}

int main() {
  RunLinkedListTest();
}
