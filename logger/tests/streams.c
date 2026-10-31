/* What logger/tests/run checks the streams of: every macro once, the one that does not return last. */
#include "logger.h"

typedef struct {
  int x;
} Opaque;

int main(void) {
  Opaque opaque = { .x = 1 };

  log_info("info");
  log_info_wnl("info without a newline\n");
  log_warn("warn");
  log_fail("fail");
  log_misc(1, "misc");
  log_fn_call();
  log_base("base");
  log_base_raw("raw\n");
  log_debug_vars("vars", opaque.x, opaque);

  log_halt("halt");
}
