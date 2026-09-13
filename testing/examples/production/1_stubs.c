/* A batch of generated stub cases, so a run's output (the per-case summary, the timings, the totals)
 * looks like a suite with dozens of cases rather than the one case in ../minimal.
 *
 * Each stub burns a different, small number of cycles so the per-case timings in the summary aren't all
 * the same number.
 */
#include "testsuite.h"

/* usNN_tcMM names must be literal identifiers (they are matched against the file's own story number at
 * registration time), so the stubs are generated via an X-macro instead of a real runtime loop: one
 * X-macro entry per case, expanded once to define the functions and once to register them. */
#define STUB_CASES(X)                                                                                                 \
  X(01, 1)                                                                                                            \
  X(02, 2)                                                                                                            \
  X(03, 3)                                                                                                            \
  X(04, 4)                                                                                                            \
  X(05, 5)                                                                                                            \
  X(06, 6)                                                                                                            \
  X(07, 7)                                                                                                            \
  X(08, 8)                                                                                                            \
  X(09, 9)                                                                                                            \
  X(10, 10)                                                                                                           \
  X(11, 11)                                                                                                           \
  X(12, 12)

/* No sleep/timer syscall here on purpose: a spin loop keeps this portable to a target with no OS to ask
 * for one, which is the whole point of the example. */
static void burn_cycles(long iterations) {
  volatile long sum = 0;
  for (long i = 0; i < iterations; ++i) {
    sum += i;
  }
}

#define DEFINE_STUB(suffix, weight)                                                                                   \
  static int us1_tc##suffix(const TestArguments *args) {                                                              \
    (void)args;                                                                                                       \
    burn_cycles((weight) * 200000L);                                                                                  \
    return 0;                                                                                                         \
  }
STUB_CASES(DEFINE_STUB)
#undef DEFINE_STUB

void register_user_story1(void) {
  START_REGISTERING();
#define REGISTER_STUB(suffix, weight) ADD_TEST_CASE(us1_tc##suffix, "Stub case " #suffix " completes", false);
  STUB_CASES(REGISTER_STUB)
#undef REGISTER_STUB
}
