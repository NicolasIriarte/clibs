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
- `testing`: a `Makefile` for building and running its bundled examples (`make -C testing`,
  `make -C testing EXAMPLE=production run`) without hand-writing the `cc`/`gen_test_registry` commands
  (#7).
- `testing`: `TESTSUITE_MAX_CASES_PER_STORY` raises the 16-cases-per-story limit at build time, and the
  bundled `Makefile` takes `LOGGER_DIR` for a copy of the library unpacked away from its logger (#10).

### Changed

- `testing`: the summary names each case, and a case number is read to its last digit, so `tc100` is
  case 100 rather than case 10 (#10).
- `testing`: the examples return whether anything failed rather than the failure count, which an exit
  status would have wrapped to 0 at 256 failures (#10).

### Fixed

- `testing`: a run whose report table could not be allocated skipped its cases and still exited 0; a
  case that cannot be run now counts as failed (#10).
- `testing`: a case's `tmp_dir` is emptied before the case runs, as documented, instead of keeping what
  an earlier run left in it (#10).
- `testing`: a case that ends the process (`exit()`, `log_halt()`) no longer takes its captured output
  with it; the run prints it and names the case (#10).
- `testing`: the summary printed a non-positive return code as a line number, mis-sorted stories
  numbered above 255, and a second `test_user_stories()` call appended to the first one's reports
  (#10).
- `testing`: two cases with the same case number in one story are refused at registration (#10).
- `testing`: the bundled `Makefile` rebuilds when a story file is removed (#10).

## [0.1.0] - 2026-09-11

### Added

- `logger`: a standalone, single-header C logger, ported from NicOs, defaulting to the same colors as
  NicOs's own (#1).
- `bash-logger`: the bash counterpart of `logger`, ported from NicOs (#1).
- `testing`: a standalone, user-story-based C test runner, ported from NicOs with its hardware- and
  project-specific pieces stripped (#1).
- CI packages each library into a versioned tarball and attaches it to the tag's release, so a specific
  version can be fetched with a plain `curl` (#1).
