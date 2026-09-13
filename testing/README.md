# testing

A small user-story-based test runner for C. Host-only (Linux/macOS): it is not meant to run cases
inside an embedded target.

## Model

Tests are grouped into "user stories", one per file. A story file is named after its number
(`4_something.c` is story 4) and defines a `register_user_story4(void)` function that registers its
cases:

```c
#include "testsuite.h"

static int us4_tc01_adds_two_numbers(const TestArguments *args) {
  (void)args;
  if (2 + 2 != 4) {
    FORCE_FAILURE();
  }
  return 0;
}

void register_user_story4(void) {
  START_REGISTERING();
  ADD_TEST_CASE(us4_tc01_adds_two_numbers, "2 + 2 equals 4", /* critical */ true);
}
```

The function-name suffix (`4`) must match the file's own leading number exactly, digit for digit --
`04_x.c` pairs with `register_user_story04`, not `register_user_story4`. Each case's own name
(`usNN_tcMM`) is checked against both the story id and, via `_tc`, its own case number.

`critical` answers one question: if this fails, are the results of everything after it meaningless? A
critical case runs first and a critical failure ends the run there, so the first line of a failing run is
the one line that mattered rather than the four hundred consequences of it. Mark few, not many -- a flag
that stops the run for an ordinary case is one people learn to ignore.

## Registering stories: `gen_test_registry`

Nothing registers itself. [`gen_test_registry`](gen_test_registry) scans a directory for `NN_*.c` files
and writes a header listing them, so an entrypoint just does:

```sh
gen_test_registry test build/inc/testsuite_registry.h
```

```c
#include "testsuite.h"
#include "testsuite_registry.h"   /* generated just above; picked up automatically if on the include path */

int main(void) {
#define __REGISTER_ONE(id) REGISTER_USER_STORY(id);
  TESTSUITE_USER_STORIES(__REGISTER_ONE)
#undef __REGISTER_ONE

  return test_user_stories();
}
```

Adding a story is then just adding a file -- nothing to register by hand and nothing to merge-conflict
over. `TESTSUITE_STORIES=4,7 gen_test_registry test ...` builds a registry for only those stories, for a
fast, narrowed run.

## Building

`testsuite.c` needs [`../logger`](../logger)'s `logger.h` on its include path (it logs through it) and a
generated `testsuite_registry.h` (see above; an empty one is used if none is found, so it still compiles
with nothing to run). There is no build system bundled for *your own* test suite -- add `testsuite.c`,
your story files, and your entrypoint to whatever you already use:

```sh
cc -I. -Ipath/to/logger -Ipath/to/generated *.c path/to/testsuite.c -o run_tests
./run_tests
```

To try the bundled examples themselves without writing any of that out by hand, use the `Makefile` in
this directory:

```sh
make -C testing                        # builds examples/minimal
make -C testing EXAMPLE=production run # builds and runs a different example
make -C testing clean
```

See [`examples/minimal`](examples/minimal) for a complete, runnable pair of files (one story, one
entrypoint) and the exact commands to build and run them by hand. See
[`examples/production`](examples/production) for a larger run -- a dozen generated stub cases, so the
summary and timings look like a suite with real volume rather than one case -- built with a custom
allocator instead of the heap (see Allocation below).

## API

| Function / macro | Purpose |
| --- | --- |
| `START_REGISTERING()` | begins a story's registration; call first inside `register_user_story<N>()` |
| `ADD_TEST_CASE(fn, criteria, critical)` | registers one case against the story being registered |
| `REGISTER_USER_STORY(id)` | declares and calls `register_user_story<id>()` |
| `FORCE_FAILURE()` | fails a case at the current line |
| `test_user_stories()` | runs every registered case, prints a summary, returns the failure count |
| `test_run_aborted()` | whether a critical failure stopped the run early |
| `test_registered_case_count()` / `test_critical_case_count()` | how many cases are registered, and how many of those are critical |
| `test_registered_user_stories()` | how many stories registered themselves |
| `test_scratch(bytes)` | borrows a zeroed buffer from a per-case arena (see `TEST_SCRATCH_MAX`) |
| `test_set_allocator(allocator)` | overrides how the suite allocates its own bookkeeping memory (see Allocation below) |

A case receives a `const TestArguments *` with one field, `tmp_dir`: a private, already-empty directory
it may write to, removed and recreated before every case runs.

## Version

`testsuite.h` defines `TESTSUITE_VERSION`, a string naming the release of CLibs this copy was cut from
(e.g. `"0.1.0"`), and its own top-of-file comment names this repository -- so a consumer who only has the
dropped-in files can still tell what it is and which version it's running.

## Allocation

The suite's own bookkeeping (the story table, the report table, the scratch arena) comes from `malloc`
by default. `test_set_allocator` overrides that with any allocator that hands out a slice of memory and
never has to give it back -- what an embedded target's external-RAM driver or a fixed static arena looks
like, for a build with no heap to call `malloc` on:

```c
static void *arena_alloc(size_t bytes, void *context) {
  /* ... hand out `bytes` from wherever this target's memory comes from ... */
}

test_set_allocator((TestAllocator){ .alloc = arena_alloc, .context = NULL });
```

Call it before anything registers a case -- registration is what triggers the first allocation. See
[`examples/production`](examples/production) for a complete example using a fixed-size bump allocator in
place of the heap.

## Configuration

Two environment variables, both optional:

- `TESTSUITE_TMP_DIR` -- where each case's private `tmp_dir` is created under. Defaults to
  `/tmp/testsuite`.
- `TESTSUITE_OUTPUT_DIR` -- where each story's run directory is created. Defaults to `build/tests`,
  relative to the current working directory.

## Differences from NicOs's test suite

This is a close port of NicOs's `test/test_suite.c`, keeping its public API and the `usNN_tcMM.c`
auto-registration convention. What did not travel, because none of it exists outside NicOs's firmware:

- The external-RAM arena driver (`workspace_dvr`) -- replaced with plain heap allocation.
- The ESP32 build path -- this is host-only from here on.
- `storage_fixture`, which reset NicOs's own app state (bookmarks, clock, wifi credentials, screens)
  between cases -- there is no equivalent here, since there is no such state to reset. A case that needs
  a clean directory should use its `tmp_dir`.
- The dependency on NicOs's own `path.h`/`str.h`/`task.h`/`helper_macros.h` libraries -- the handful of
  functions actually needed are vendored as internal (non-public) helpers in `testsuite.c` instead.
