/* Entrypoint for the fixture suite. See ../run for what each FIXTURE_* variable is checking. */
#include "testsuite.h"

#include "testsuite_registry.h"

#include <stdio.h>
#include <stdlib.h>

/* Hands out the first allocation (the story table) and refuses every one after it (the report table). */
static void *only_the_first(size_t bytes, void *context) {
  static int calls = 0;
  (void)context;
  return (++calls == 1) ? malloc(bytes) : NULL;
}

int main(void) {
  if (getenv("FIXTURE_NO_REPORTS") != NULL) {
    test_set_allocator((TestAllocator){ .alloc = only_the_first, .context = NULL });
  }

#define __REGISTER_ONE(id) REGISTER_USER_STORY(id);
  TESTSUITE_USER_STORIES(__REGISTER_ONE)
#undef __REGISTER_ONE

  int failures = test_user_stories();

  if (getenv("FIXTURE_TWICE") != NULL) {
    failures = test_user_stories();
    fprintf(stderr, "second run failures=%d\n", failures);
  }

  return failures != 0;
}
