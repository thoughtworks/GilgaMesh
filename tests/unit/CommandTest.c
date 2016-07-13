#include "command.h"

#include "cmocka_includes.h"

static int setup(void **state) {
  command_initialize();
  return 0;
}

static void foo_callback(char **parsedCommandArray) {
  check_expected_ptr(parsedCommandArray);
}


static void test_command_responds_to_bro() {
  char* data[2] = { "bro", "foo" };

  expect_value(broadcast_logging_message, parsedCommandArray, data);
  command_execute(data, 2);
}

static void test_command_responds_to_cons() {
  char* data[1] = { "cons" };

  expect_value(print_all_connections, parsedCommandArray, data);
  command_execute(data, 1);
}

static void test_command_responds_to_adv() {
  char* data[1] = { "adv" };

  expect_value(check_advertising_status, parsedCommandArray, data);
  command_execute(data, 1);
}

static void test_command_responds_to_scan() {
  char* data[1] = { "scan" };

  expect_value(check_scanning_status, parsedCommandArray, data);
  command_execute(data, 1);
}

static void test_command_responds_to_hb() {
  char* data[2] = { "hb", "on" };

  expect_value(toggle_heartbeats, parsedCommandArray, data);
  command_execute(data, 2);
}

static void test_command_responds_to_help() {
  char* data[1] = { "help" };

  expect_any_count(terminal_putstring, string, 2);
  command_execute(data, 1);
}

static void test_command_prints_help_when_command_is_unrecognized() {
  char* data[1] = { "xyz" };

  expect_any_count(terminal_putstring, string, 2);
  command_execute(data, 1);
}

static void test_command_responds_to_new_commands() {
  char* data[1] = { "foo" };

  expect_any_count(terminal_putstring, string, 2);
  command_execute(data, 1);

  mesh_add_terminal_command("foo", "Does some stuff", foo_callback);
  expect_value(foo_callback, parsedCommandArray, data);
  command_execute(data, 1);
}

int RunCommandTest(void) {
  const struct CMUnitTest tests[] = {
//          cmocka_unit_test(test_command_responds_to_bro),
//          cmocka_unit_test(test_command_responds_to_cons),
//          cmocka_unit_test(test_command_responds_to_adv),
//          cmocka_unit_test(test_command_responds_to_scan),
//          cmocka_unit_test(test_command_responds_to_hb),
          cmocka_unit_test(test_command_responds_to_help),
          cmocka_unit_test(test_command_prints_help_when_command_is_unrecognized),
          cmocka_unit_test(test_command_responds_to_new_commands),
  };

  return cmocka_run_group_tests_name("CommandTest", tests, setup, NULL);
}

int main() {
  return RunCommandTest();
}