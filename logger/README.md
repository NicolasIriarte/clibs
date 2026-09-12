# logger

A single-header, level-gated C logger with a `_Generic`-based variable printer. Standalone on purpose:
drop `logger.h` into a project and it compiles as-is.

## Usage

Copy `logger.h` into your project (or add this directory to your include path) and include it:

```c
#include "logger.h"

int main(void) {
  log_info("starting up");
  log_warn("value out of range: %d", 42);
  log_fail("could not open the file");
  return 0;
}
```

See [`examples/minimal.c`](examples/minimal.c) for a runnable version, including `log_debug_vars()`.

## API

| Macro | Purpose |
| --- | --- |
| `log_info(fmt, ...)` | informational message |
| `log_warn(fmt, ...)` | warning |
| `log_fail(fmt, ...)` | failure (does not stop the program) |
| `log_halt(fmt, ...)` | prints then calls `exit(1)` |
| `log_misc(level, fmt, ...)` | printed only when `level <= LOGGER_TRACE_LEVEL` |
| `log_base(fmt, ...)` / `log_base_raw(fmt, ...)` | plain `printf`, with/without a trailing newline |
| `log_debug_vars(msg, ...)` | prints each variable's name, value, and hex, dispatched by type |
| `log_fn_call()` | shorthand for `log_misc(50, "Function \"%s\" was called.", __func__)` |

Each level is gated by an `..._ENABLED` flag and can be compiled out entirely.

## Version

`logger.h` defines `LOGGER_VERSION`, a string naming the release of CLibs this copy was cut from (e.g.
`"0.1.0"`), and its own top-of-file comment names this repository -- so a consumer who only has the
dropped-in file can still tell what it is and which version it's running.

## Configuration

Every knob has a default and works out of the box. To override one, add a `logger_config.h` anywhere on
your include path -- `logger.h` picks it up automatically if present:

```c
// logger_config.h
#define LOGGER_TRACE_LEVEL (10)   // log_misc() calls above this level are dropped
#define LOGGER_MISC_ENABLED (0)   // compile log_misc() out entirely
#define COLOR_RED ""              // disable colour for one level ("" for all COLOR_* disables it globally)
```

Available knobs: `LOGGER_INFO_ENABLED`, `LOGGER_WARN_ENABLED`, `LOGGER_FAIL_ENABLED`,
`LOGGER_MISC_ENABLED`, `LOGGER_TRACE_LEVEL`, `COLOR_DEFAULT`, `COLOR_SEAGREEN`, `COLOR_WHITE`,
`COLOR_GREEN`, `COLOR_YELLOW`, `COLOR_RED`.

### Default colors

Out of the box, `logger.h` uses the same ANSI colors as [`../bash-logger`](../bash-logger), so a C
program and a shell script report the same way:

| Level | Color |
| --- | --- |
| `[INFO]` | green |
| `[WARN]` | yellow |
| `[FAIL]` / `[HALT]` | red |
| `[MISC]` | white |
| `[file:line]` | seagreen (`#00b6b6`) |

### Printing your own types

`log_debug_vars()` dispatches on the argument's type via `_Generic`. To make it print your own struct,
define `PRINTABLE_TYPES` in `logger_config.h` before `logger.h` is included:

```c
void print_point(const char *name, Point p, int size);

#define PRINTABLE_TYPES TYPE_CALLBACK(Point, print_point)
```

`TYPE_CALLBACK` also generates pointer and const-pointer overloads, so `Point *` and `const Point *`
become printable for free.
