#pragma once

/**
 * logger.h -- part of CLibs (http://100.125.208.18:3000/nico/CLibs). See LOGGER_VERSION below for the exact
 * release this copy was cut from.
 *
 * A small level-gated logger, with a `_Generic`-based variable printer.
 *
 * Standalone on purpose: drop this file into another project and it compiles as-is, with every knob
 * below falling back to a sensible default. The defaults below match this project's own bash logger
 * (see ../bash-logger/logger.bashrc), so a C program and a shell script sitting side by side print the
 * same colors.
 *
 * To customise it, provide a `logger_config.h` anywhere on the include path; it is picked up
 * automatically if present. That file may define any of:
 *
 *   - LOGGER_INFO_ENABLED / LOGGER_WARN_ENABLED / LOGGER_FAIL_ENABLED / LOGGER_MISC_ENABLED
 *   - LOGGER_TRACE_LEVEL   -- log_misc() prints only at or below this level
 *   - COLOR_*              -- ANSI escapes, or empty strings to disable colour entirely
 *   - PRINTABLE_TYPES      -- TYPE_CALLBACK() entries making your own types printable by
 *                             log_debug_vars(); see the bottom of this file
 */

#if defined(__has_include)
  #if __has_include("logger_config.h")
    #include "logger_config.h"
  #endif
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Version. Not a knob, and not overridable from logger_config.h: this is which release of CLibs this
 * copy of logger.h was cut from, for a consumer that needs to know what it's running.
 * ------------------------------------------------------------------------- */
#define LOGGER_VERSION "0.1.0"

/* ---------------------------------------------------------------------------
 * Defaults. Every one of these is overridable from logger_config.h.
 * ------------------------------------------------------------------------- */

#ifndef LOGGER_INFO_ENABLED
  #define LOGGER_INFO_ENABLED (1)
#endif
#ifndef LOGGER_WARN_ENABLED
  #define LOGGER_WARN_ENABLED (1)
#endif
#ifndef LOGGER_FAIL_ENABLED
  #define LOGGER_FAIL_ENABLED (1)
#endif
#ifndef LOGGER_MISC_ENABLED
  #define LOGGER_MISC_ENABLED (1)
#endif
#ifndef LOGGER_TRACE_LEVEL
  #define LOGGER_TRACE_LEVEL (50)
#endif

/* Set an empty string for any of these in logger_config.h to disable colour entirely. */
#ifndef COLOR_DEFAULT
  #define COLOR_DEFAULT "\x1b[0m"
#endif
#ifndef COLOR_SEAGREEN
  #define COLOR_SEAGREEN "\x1b[38;2;0;182;182m"
#endif
#ifndef COLOR_WHITE
  #define COLOR_WHITE "\033[1;37m"
#endif
#ifndef COLOR_GREEN
  #define COLOR_GREEN "\033[1;32m"
#endif
#ifndef COLOR_YELLOW
  #define COLOR_YELLOW "\033[1;33m"
#endif
#ifndef COLOR_RED
  #define COLOR_RED "\033[1;31m"
#endif

/* No custom printable types unless the consumer supplied some. Also what keeps this compiling if
 * logger_config.h is reached through an include cycle and has not finished parsing yet. */
#ifndef PRINTABLE_TYPES
  #define PRINTABLE_TYPES
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Logs an informational message.
 *
 * Prints a formatted message with INFO level, including file and line context,
 * if informational logging is enabled.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_info(fmt, ...)                                                                                             \
  do {                                                                                                                 \
    if (LOGGER_INFO_ENABLED) {                                                                                         \
      printf(                                                                                                          \
        COLOR_GREEN "[INFO] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__ \
      );                                                                                                               \
    }                                                                                                                  \
  } while (0)

/**
 * @brief Logs an informational message without newline at the end.
 *
 * Prints a formatted message with INFO level, including file and line context,
 * if informational logging is enabled.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_info_wnl(fmt, ...)                                                                                         \
  do {                                                                                                                 \
    if (LOGGER_INFO_ENABLED) {                                                                                         \
      printf(                                                                                                          \
        COLOR_GREEN "[INFO] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt, __FILE__, __LINE__, ##__VA_ARGS__      \
      );                                                                                                               \
    }                                                                                                                  \
  } while (0)


/**
 * @brief Logs a warning message.
 *
 * Prints a formatted message with WARN level, including file and line context,
 * if warning logging is enabled.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_warn(fmt, ...)                                                                                             \
  do {                                                                                                                 \
    if (LOGGER_WARN_ENABLED) {                                                                                         \
      printf(                                                                                                          \
        COLOR_YELLOW "[WARN] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt "\n",                                  \
        __FILE__,                                                                                                      \
        __LINE__,                                                                                                      \
        ##__VA_ARGS__                                                                                                  \
      );                                                                                                               \
    }                                                                                                                  \
  } while (0)

/**
 * @brief Logs a failure message.
 *
 * Prints a formatted message with FAIL level, including file and line context,
 * if failure logging is enabled.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_fail(fmt, ...)                                                                                             \
  do {                                                                                                                 \
    if (LOGGER_FAIL_ENABLED) {                                                                                         \
      printf(                                                                                                          \
        COLOR_RED "[FAIL] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__   \
      );                                                                                                               \
    }                                                                                                                  \
  } while (0)

/**
 * @brief Logs a critical error and halts execution.
 *
 * Prints a formatted message with HALT level and terminates the program.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_halt(fmt, ...)                                                                                             \
  do {                                                                                                                 \
    printf(                                                                                                            \
      COLOR_RED "[HALT] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__     \
    );                                                                                                                 \
    exit(1);                                                                                                           \
  } while (0)

/**
 * @brief Logs a miscellaneous message with level filtering.
 *
 * Prints a formatted message with MISC level if enabled and within the
 * configured trace level threshold.
 *
 * @param[in] level Logging level used for filtering.
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_misc(level, fmt, ...)                                                                                      \
  do {                                                                                                                 \
    if (LOGGER_MISC_ENABLED && (level) <= LOGGER_TRACE_LEVEL) {                                                        \
      printf(                                                                                                          \
        COLOR_WHITE "[MISC] " COLOR_SEAGREEN "[%s:%d]" COLOR_DEFAULT " - " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__ \
      );                                                                                                               \
    }                                                                                                                  \
  } while (0)

/**
 * @brief Logs a basic message.
 *
 * Prints a formatted message followed by a newline without any metadata.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_base(fmt, ...)                                                                                             \
  do {                                                                                                                 \
    printf(fmt "\n", ##__VA_ARGS__);                                                                                   \
  } while (0)

/**
 * @brief Logs a raw message without newline.
 *
 * Prints a formatted message without appending a newline.
 *
 * @param[in] fmt Format string.
 * @param[in] ... Optional arguments for formatting.
 * @return None.
 */
#define log_base_raw(fmt, ...)                                                                                         \
  do {                                                                                                                 \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
  } while (0)


/**
 * @brief Logs a function call trace.
 *
 * Prints a message indicating that the current function was called.
 *
 * @return None.
 */
#define log_fn_call() log_misc(50, "Function \"%s\" was called.", __func__)


/* =========================================================================
 * Internal Helper Printers (static inline: zero symbol leakage / overhead)
 * ========================================================================= */
/* `size` is the width in hex digits of the type the value came from, which for `signed char` and `short`
 * is narrower than the `int` they arrive as. Masked to that width, so -1 reads 0xFF rather than
 * sign-extended to 0xFFFFFFFF. */
static inline void _dbg_print_int(const char *name, int val, int size) {
  const unsigned int mask = (size < (int)(sizeof(unsigned int) * 2)) ? ((1U << (size * 4)) - 1U) : ~0U;
  log_base("%s = %d (0x%0*X)", name, val, size, (unsigned int)val & mask);
}
static inline void _dbg_print_uint(const char *name, unsigned int val, int size) {
  log_base("%s = %u (0x%0*X)", name, val, size, val);
}
static inline void _dbg_print_long(const char *name, long val, int size) {
  log_base("%s = %ld (0x%0*lx)", name, val, size, val);
}
static inline void _dbg_print_ulong(const char *name, unsigned long val, int size) {
  log_base("%s = %lu (0x%0*lx)", name, val, size, val);
}
static inline void _dbg_print_longlong(const char *name, long long val, int size) {
  log_base("%s = %lld (0x%0*llx)", name, val, size, val);
}
static inline void _dbg_print_ulonglong(const char *name, unsigned long long val, int size) {
  log_base("%s = %llu (0x%0*llx)", name, val, size, val);
}
static inline void _dbg_print_double(const char *name, double val, int size) {
  (void)size; /* Width is meaningless for a float, but the dispatch table's signature is shared. */
  log_base("%s = %f", name, val);
}
static inline void _dbg_print_char(const char *name, char val, int size) {
  log_base("%s = '%c' (0x%0*X)", name, val, size, (unsigned int)(unsigned char)val);
}
static inline void _dbg_print_bool(const char *name, bool val, int size) {
  log_base("%s = %s (0x%0*X)", name, val ? "true" : "false", size, (unsigned int)val);
}
static inline void _dbg_print_str(const char *name, const char *val, int size) {
  (void)size; /* Width is meaningless for a string, but the dispatch table's signature is shared. */
  log_base("%s = \"%s\"", name, val ? val : "NULL");
}
static inline void _dbg_print_ptr(const char *name, const void *val, int size) {
  log_base("%s = %*p", name, size, val);
}
/* Variadic fallback safely accepts ANY type (including structs) without error
 */
static inline void _dbg_print_unknown(const char *name, ...) {
  printf("%s = <unknown type>\n", name);
}

/* =========================================================================
 * Type Dispatcher via _Generic Function Selection
 * ========================================================================= */
#define CAT(a, b) CAT_I(a, b)
#define CAT_I(a, b) a##b

/*
 * Given something like:
 *   void _dbg_print_point(const char *name, Point val, int size);
 *
 * This generates the pairs:
 *   void _dbg_print_point_ptr(const char *name, Point *val, int size);
 *   void _dbg_print_point_cptr(const char *name, const Point *val, int size);
 */

#define TYPE_CALLBACK(type, fnName)                                                                                    \
  static inline void CAT(fnName, _ptr)(const char *name, type *val, int size) {                                        \
    if (val != NULL) {                                                                                                 \
      fnName(name, *val, size);                                                                                        \
    } else {                                                                                                           \
      _dbg_print_ptr(name, val, size);                                                                                 \
    }                                                                                                                  \
  }                                                                                                                    \
  static inline void CAT(fnName, _cptr)(const char *name, const type *val, int size) {                                 \
    if (val != NULL) {                                                                                                 \
      fnName(name, *val, size);                                                                                        \
    } else {                                                                                                           \
      _dbg_print_ptr(name, val, size);                                                                                 \
    }                                                                                                                  \
  }

PRINTABLE_TYPES
#undef TYPE_CALLBACK


/* clang-format off */
#define TYPE_CALLBACK(type, fnName) \
  type: fnName,                     \
  type *: CAT(fnName, _ptr),        \
  const type *: CAT(fnName, _cptr),
/* clang-format on */

#define print_one(x)                                                                                                   \
  _Generic((x),                                                                                                        \
    PRINTABLE_TYPES /* Custom types added by the user */                                                               \
                                                                                                                       \
    /* All standard types */                                                                                           \
    bool: _dbg_print_bool,                                                                                             \
                                                                                                                       \
    char: _dbg_print_char,                                                                                             \
    signed char: _dbg_print_int,                                                                                       \
    unsigned char: _dbg_print_uint,                                                                                    \
                                                                                                                       \
    short: _dbg_print_int,                                                                                             \
    unsigned short: _dbg_print_uint,                                                                                   \
                                                                                                                       \
    int: _dbg_print_int,                                                                                               \
    unsigned int: _dbg_print_uint,                                                                                     \
                                                                                                                       \
    long: _dbg_print_long,                                                                                             \
    unsigned long: _dbg_print_ulong,                                                                                   \
                                                                                                                       \
    long long: _dbg_print_longlong,                                                                                    \
    unsigned long long: _dbg_print_ulonglong,                                                                          \
                                                                                                                       \
    float: _dbg_print_double,                                                                                          \
    double: _dbg_print_double,                                                                                         \
                                                                                                                       \
    char *: _dbg_print_str,                                                                                            \
    const char *: _dbg_print_str,                                                                                      \
                                                                                                                       \
    void *: _dbg_print_ptr,                                                                                            \
    const void *: _dbg_print_ptr,                                                                                      \
                                                                                                                       \
    int *: _dbg_print_ptr,                                                                                             \
    const int *: _dbg_print_ptr,                                                                                       \
                                                                                                                       \
    double *: _dbg_print_ptr,                                                                                          \
    const double *: _dbg_print_ptr,                                                                                    \
                                                                                                                       \
    default: _dbg_print_unknown)("  " #x, x, sizeof(x) * 2)

/* =========================================================================
 * Variadic Argument Iterator (Supports up to 8 arguments cleanly)
 * ========================================================================= */
#define ARG_COUNT(...) ARG_COUNT_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define ARG_COUNT_(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#define FOR_EACH_1(m, x) m(x)
#define FOR_EACH_2(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_1(m, __VA_ARGS__)
#define FOR_EACH_3(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_2(m, __VA_ARGS__)
#define FOR_EACH_4(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_3(m, __VA_ARGS__)
#define FOR_EACH_5(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_4(m, __VA_ARGS__)
#define FOR_EACH_6(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_5(m, __VA_ARGS__)
#define FOR_EACH_7(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_6(m, __VA_ARGS__)
#define FOR_EACH_8(m, x, ...)                                                                                          \
  m(x);                                                                                                                \
  FOR_EACH_7(m, __VA_ARGS__)

#define FOR_EACH(m, ...) CAT(FOR_EACH_, ARG_COUNT(__VA_ARGS__))(m, __VA_ARGS__)

/* =========================================================================
 * Clean Public Macro
 * ========================================================================= */
#define log_debug_vars(msg, ...)                                                                                       \
  do {                                                                                                                 \
    /* Gated as a whole: the variables are part of the INFO message. */                                                \
    if (!(LOGGER_INFO_ENABLED)) {                                                                                      \
      break;                                                                                                           \
    }                                                                                                                  \
    /* Special case of a single variable */                                                                            \
    if (ARG_COUNT(__VA_ARGS__) == 1) {                                                                                 \
      log_info_wnl(msg);                                                                                               \
      FOR_EACH(print_one, __VA_ARGS__);                                                                                \
    } else {                                                                                                           \
      log_info(msg);                                                                                                   \
      log_base("{");                                                                                                   \
      FOR_EACH(print_one, __VA_ARGS__);                                                                                \
      log_base("}");                                                                                                   \
    }                                                                                                                  \
  } while (0)

#ifdef __cplusplus
}
#endif
