#pragma once

/**
 * testsuite.h -- part of CLibs (<forgejo-url>/nico/CLibs). See TESTSUITE_VERSION below for the exact
 * release this copy was cut from.
 *
 * A small user-story-based test runner for C.
 *
 * Tests are grouped into "user stories", one per file, registered automatically from the file's leading
 * number (`test/04_something.c` registers story 4). Within a story, cases are named `usNN_tcMM` and
 * discovered the same way. A generated registry (see gen_test_registry) lists which story files exist,
 * so adding a story means adding a file -- nothing to register by hand and nothing to merge-conflict
 * over.
 *
 * Host-only by design: this runs wherever a C compiler and a POSIX-ish libc do (Linux, macOS). It is not
 * meant to run cases inside an embedded target.
 */

//- System includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//- Macros constants

/** @brief: Which release of CLibs this copy of testsuite.h was cut from. Not a knob. */
#define TESTSUITE_VERSION "0.1.0"

#define FORCE_FAILURE() return __LINE__

//- Macros functions

#define START_REGISTERING() UserStory *user_story = __get_next_user_story(__FILE__)

/** @brief: Registers one case against the story being registered.
 *
 * `critical` is not "this feature matters". It answers one question:
 *
 *     if this fails, are the results of everything after it meaningless?
 *
 * The scheduler running, the framework itself working, the one place that hands out working memory --
 * those are critical, because a suite where any of them is broken reports several hundred consequences
 * and not one cause. A feature, however important to a reader, is not: one screen failing to render says
 * nothing about whether another one still does.
 *
 * Critical cases run first, before everything else, and a critical failure ends the run there. That is
 * the whole reason to mark one: it is the difference between reading the first line of a run and reading
 * all four hundred.
 *
 * Marking too many is worse than marking none: a flag that stops the run for an ordinary feature is a
 * flag people learn to ignore. */
#define ADD_TEST_CASE(fnName, acceptance_criteria, critical)                                                           \
  __add_test_case(user_story, #fnName, fnName, acceptance_criteria, critical, __LINE__)

#define REGISTER_USER_STORY(id)                                                                                        \
  extern void register_user_story##id();                                                                               \
  register_user_story##id()

//- Type definitions

typedef struct {
  /** @brief: A private, already-created directory this case may write to. Removed and recreated before
   * every case runs, so a case never sees what an earlier one left behind. */
  const char *tmp_dir;
} TestArguments;

typedef int (*TestSignature)(const TestArguments *);
typedef struct UserStory UserStory;

//- Public functions

UserStory *__get_next_user_story(const char *fileName);

void __add_test_case(
  UserStory *us,
  const char *fnName,
  TestSignature testCb,
  const char *acceptanceCriteria,
  bool isCritical,
  uint32_t lineNum
);

/**
 * @brief Runs every registered test case and prints the summary.
 *
 * @return The number of test cases that failed (0 if all passed).
 */
int test_user_stories(void);

/**
 * @brief Whether the last run stopped early because a critical case failed.
 *
 * @return `true` if a critical case failed and the rest of the run was skipped.
 */
bool test_run_aborted(void);

/**
 * @brief How many cases are registered across every story.
 *
 * @return The count.
 */
size_t test_registered_case_count(void);

/**
 * @brief How many of those are marked critical.
 *
 * @return The count.
 */
int test_critical_case_count(void);

/**
 * @brief How many user stories have registered themselves.
 *
 * @return The count.
 */
size_t test_registered_user_stories(void);

/**
 * @brief How many bytes the story table occupies.
 *
 * Zero means it was never allocated, which is the answer that should fail a case checking for it.
 *
 * @return The bytes, or zero if the table was never allocated.
 */
size_t test_stories_heap_bytes(void);

/**
 * @brief Borrows a working buffer for the duration of one test case.
 *
 * One case runs at a time, so one arena serves all of them. Handed back and zeroed before every case, so
 * a case never sees what an earlier one left in it.
 *
 * **Each call returns its own bytes**, carved off the arena. Two calls within one case do not overlap.
 * The arena is handed back at the start of every case, so a pointer does not outlive the case that asked
 * for it; anything that must, needs its own storage.
 *
 * @param[in] bytes How much is needed. More than the arena has left in this case is refused.
 * @return The buffer, or `NULL` if it could not be taken. A caller must handle `NULL`.
 */
void *test_scratch(size_t bytes);

/** @brief: How much one case may borrow in total, across however many calls it makes. */
#define TEST_SCRATCH_MAX (48 * 1024)

#ifdef __cplusplus
}
#endif
