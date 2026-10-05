/* A story numbered far past what one byte holds, for the summary's ordering to be checked against. */
#include "testsuite.h"

static int us70000_tc01_passes(const TestArguments *args) {
  (void)args;
  return 0;
}

void register_user_story70000(void) {
  START_REGISTERING();
  ADD_TEST_CASE(us70000_tc01_passes, "A story number may be large", false);
}
