# CLibs

Small, standalone C (and bash) libraries, each meant to be dropped into another project on its own.
Nothing in one library depends on another project this code grew up in -- see each library's own README
for what it needs and how to use it. The one dependency inside this repository is deliberate: `testing`
logs through `logger`, so it needs `logger.h` alongside it.

## Libraries

| Library | What it is |
| --- | --- |
| [`logger`](logger) | A single-header, level-gated C logger with a `_Generic`-based variable printer. |
| [`bash-logger`](bash-logger) | The bash counterpart of `logger` -- same levels, same output shape. |
| [`testing`](testing) | A user-story-based test runner for C, for a Linux host. Needs `logger`. |

Each has a minimal, runnable example under its own `examples/` directory.

## Getting a released version

Every tag publishes one versioned tarball per library, attached to that tag's release. Fetch one
directly:

```sh
curl -LO http://100.125.208.18:3000/nico/CLibs/releases/download/vX.Y.Z/logger-vX.Y.Z.tar.gz
tar xzf logger-vX.Y.Z.tar.gz
```

Replace `logger` with `bash-logger` or `testing` for the other libraries, and `vX.Y.Z` with the version
you want -- see this repository's releases for what exists. A tag ending in `-N` (`v0.2.0-1`) is the same
release as the one it is named after, cut again for a change that is not functional.

## Working on this repo

This project uses [`just`](https://just.systems) as its command runner:

```sh
just           # list recipes
just test      # build and run every library's example
just test_package  # check that `just package` produces what it should
just test_tooling  # check the release scripts under bash/
just lint          # shellcheck every shell file
```

See [`GUIDELINES.md`](GUIDELINES.md) for how this project is worked on -- branch and PR conventions,
what requires an issue, and what an AI model may and may not do on its own.
