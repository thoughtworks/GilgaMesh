#include "app/buzzer.h"
#include "boards.h"

#include "cmocka_includes.h"

static void Buzzer_initialize_creates_timer_and_clears_pin() {
  expect_any(create_single_shot_timer, timer_id);
  expect_value(nrf_gpio_pin_clear, pin_number, BUZZER_PIN_NUMBER);

  buzzer_initialize();
}

int RunBuzzerTest(void) {
  const struct CMUnitTest tests[] = {
          cmocka_unit_test(Buzzer_initialize_creates_timer_and_clears_pin),
  };

  return cmocka_run_group_tests_name("BuzzerTest", tests, NULL, NULL);
}

int main() {
  return RunBuzzerTest();
}
