/* Bare-minimum usage of logger.h. Build and run with:
 *
 *   cc -I.. minimal.c -o minimal && ./minimal
 */
#include "logger.h"

typedef struct {
  int x;
  int y;
} Point;

int main(void) {
  log_info("starting up");
  log_warn("this is a warning about %s", "something");
  log_fail("this is what a failure looks like");

  int count = 3;
  log_debug_vars("counters", count);

  Point p = { .x = 1, .y = 2 };
  log_debug_vars("point", p.x, p.y);

  return 0;
}
