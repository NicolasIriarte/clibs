# Changelog

All notable changes to CLibs are recorded here.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and the project uses
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). Entries reference the issue they came from.

## [Unreleased]

### Added

- `logger`: a standalone, single-header C logger, ported from NicOs, defaulting to the same colors as
  NicOs's own (#1).
- `bash-logger`: the bash counterpart of `logger`, ported from NicOs (#1).
- `testing`: a standalone, user-story-based C test runner, ported from NicOs with its hardware- and
  project-specific pieces stripped (#1).
- CI packages each library into a versioned tarball and attaches it to the tag's release, so a specific
  version can be fetched with a plain `curl` (#1).
