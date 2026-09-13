# Changelog

All notable changes to CLibs are recorded here.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and the project uses
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). Entries reference the issue they came from.

## [Unreleased]

### Added

- Each library now exposes its own version to consumers: `LOGGER_VERSION` (`logger.h` and
  `logger.bashrc`) and `TESTSUITE_VERSION` (`testsuite.h`), plus a top-of-file header naming this
  repository, so a dropped-in copy can be identified on its own (#5).
- `testing`: `test_set_allocator` lets a caller replace the suite's own bookkeeping allocations (the
  story table, the report table, the scratch arena) with a custom allocator instead of `malloc`, so it
  can run somewhere with no heap to call it on (#7).
- `testing`: per-case timing, shown next to each case in the summary (#7).
- `testing`: a new `examples/production` example -- a dozen generated stub cases running through a
  custom bump allocator, for a look at the suite's output at production-sized volume (#7).

## [0.1.0] - 2026-09-11

### Added

- `logger`: a standalone, single-header C logger, ported from NicOs, defaulting to the same colors as
  NicOs's own (#1).
- `bash-logger`: the bash counterpart of `logger`, ported from NicOs (#1).
- `testing`: a standalone, user-story-based C test runner, ported from NicOs with its hardware- and
  project-specific pieces stripped (#1).
- CI packages each library into a versioned tarball and attaches it to the tag's release, so a specific
  version can be fetched with a plain `curl` (#1).
