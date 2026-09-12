# bash-logger

A small level-gated logger for bash scripts. Standalone on purpose: source `logger.bashrc` and it works,
with every knob already defaulted. It is the bash counterpart of [`../logger`](../logger) -- same
function names, same level prefixes, same `"[LEVEL] [file:line] - message"` shape, and the same
`printf`-style calling convention, so a script and a C source file report the same thing the same way.

## Usage

```sh
source logger.bashrc

log_info "starting up"
log_warn "value out of range: %d" 42
log_fail "could not open %s" "$file"
```

See [`examples/minimal.sh`](examples/minimal.sh) for a runnable version.

## API

| Function | Purpose |
| --- | --- |
| `log_info fmt [args...]` | informational message |
| `log_warn fmt [args...]` | warning |
| `log_fail fmt [args...]` | failure (does not exit the script) |
| `log_halt fmt [args...]` | prints then calls `exit 1` |
| `log_misc level fmt [args...]` | printed only when `level <= LOGGER_TRACE_LEVEL` |

There is no `log_base`: that exists in the C logger to print without decoration, which in bash is just
`printf`.

## Version

`logger.bashrc` exports `LOGGER_VERSION`, a string naming the release of CLibs this copy was cut from
(e.g. `"0.1.0"`), and its own top-of-file comment names this repository -- so a consumer who only has the
dropped-in file can still tell what it is and which version it's running.

## Configuration

Every color and `log_misc` knob has a default. Export a variable before sourcing `logger.bashrc` to
override it:

```sh
export LOGGER_TRACE_LEVEL=10   # log_misc calls above this level are dropped
export LOGGER_MISC_ENABLED=0   # silence log_misc entirely
export COLOR_RED=""            # disable colour for one level ("" for all COLOR_* disables it globally)
source logger.bashrc
```

Available variables: `LOGGER_MISC_ENABLED`, `LOGGER_TRACE_LEVEL`, `COLOR_DEFAULT`, `COLOR_SEAGREEN`,
`COLOR_WHITE`, `COLOR_GREEN`, `COLOR_YELLOW`, `COLOR_RED`.

### Default colors

The defaults match [`../logger`](../logger)'s C defaults exactly:

| Level | Color |
| --- | --- |
| `[INFO]` | green |
| `[WARN]` | yellow |
| `[FAIL]` / `[HALT]` | red |
| `[MISC]` | white |
| `[file:line]` | seagreen (`#00b6b6`) |
