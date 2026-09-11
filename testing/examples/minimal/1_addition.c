/* One "user story" per file, named after its leading number: this file is story 1. Test case names
 * follow `usNN_tcMM`, matched against that same number. */
#include "testsuite.h"

static int us1_tc01_adds_two_numbers(const TestArguments *args) {
  (void)args;

  int result = 2 + 2;
  if (result != 4) {
    FORCE_FAILURE();
  }
  return 0;
}

void register_user_story1(void) {
  START_REGISTERING();
  ADD_TEST_CASE(us1_tc01_adds_two_numbers, "2 + 2 equals 4", true);
}
