/* The story ../run drives. Each case misbehaves only when its FIXTURE_* variable is set, so one binary
 * covers every scenario and an unset environment is a plain passing run. */
#include "testsuite.h"

#include "logger.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int us1_tc01_finds_its_tmp_dir_empty(const TestArguments *args) {
  char path[512];
  snprintf(path, sizeof(path), "%s/left_behind", args->tmp_dir);

  if (access(path, F_OK) == 0) {
    printf("found what an earlier run left behind\n");
    FORCE_FAILURE();
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    FORCE_FAILURE();
  }
  fclose(file);
  return 0;
}

static int us1_tc02_returns_negative(const TestArguments *args) {
  (void)args;
  return (getenv("FIXTURE_NEGATIVE") != NULL) ? -1 : 0;
}

static int us1_tc03_exits(const TestArguments *args) {
  (void)args;
  if (getenv("FIXTURE_EXIT") != NULL) {
    printf("said before exiting\n");
    exit(3);
  }
  return 0;
}

/* log_fail goes to stderr: capturing has to keep it, and keep it in order with stdout. */
static int us1_tc04_logs(const TestArguments *args) {
  (void)args;
  printf("printed first\n");
  log_fail("logged second");
  printf("printed third\n");
  return (getenv("FIXTURE_LOGS") != NULL) ? -1 : 0;
}

static int us1_tc05_aborts(const TestArguments *args) {
  (void)args;
  if (getenv("FIXTURE_ABORT") != NULL) {
    printf("said before aborting\n");
    assert(0 && "aborted on purpose");
  }
  return 0;
}

/* Case 100, not case 10: the number is read to its last digit. */
static int us1_tc100_has_three_digits(const TestArguments *args) {
  (void)args;
  return 0;
}

static int us1_tc10_has_two_digits(const TestArguments *args) {
  (void)args;
  return 0;
}

static int us1_tc01_again(const TestArguments *args) {
  (void)args;
  return 0;
}

void register_user_story1(void) {
  START_REGISTERING();
  ADD_TEST_CASE(us1_tc01_finds_its_tmp_dir_empty, "A case's tmp_dir starts empty", false);
  ADD_TEST_CASE(us1_tc02_returns_negative, "A case may return a negative code", false);
  ADD_TEST_CASE(us1_tc03_exits, "A case may end the process", false);
  ADD_TEST_CASE(us1_tc04_logs, "A case's log lines are part of its output", false);
  ADD_TEST_CASE(us1_tc05_aborts, "A case may abort", false);
  ADD_TEST_CASE(us1_tc100_has_three_digits, "A case number may have three digits", false);
  ADD_TEST_CASE(us1_tc10_has_two_digits, "And is not mistaken for a shorter one", false);
  if (getenv("FIXTURE_DUPLICATE") != NULL) {
    ADD_TEST_CASE(us1_tc01_again, "A case number may not be used twice", false);
  }
}
