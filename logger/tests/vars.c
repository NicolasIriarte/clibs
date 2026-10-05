/* What logger/tests/run compiles and checks the output of. Each line it prints is asserted on there. */
#include "logger.h"

int main(void) {
  char c = 'A';
  signed char sc = -1;
  short sh = -2;
  unsigned long long ull = 0xFFFFFFFFFFFFFFFFULL;
  bool b = true;
  log_debug_vars("vars", c, sc, sh, ull, b);

  int count = 3;
  log_debug_vars("single", count);

  /* An expression as the level, above the default trace level of 50 on the branch taken. */
  log_misc(count ? 100 : 1, "misc above the trace level");
  log_misc(count ? 1 : 100, "misc below the trace level");

  return 0;
}
