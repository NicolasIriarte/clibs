#!/bin/bash

###################################################################################################################
# logger.bashrc -- part of CLibs (http://100.125.208.18:3000/nico/CLibs). See LOGGER_VERSION below for the exact
# release this copy was cut from.
#
# A small level-gated logger for bash scripts. The bash half of ../logger/logger.h -- same function names, same
# level prefixes, same "[LEVEL] [file:line] - message" shape, and the same printf-style calling convention, so a
# script and a source file report the same thing the same way.
#
# Standalone on purpose: source this file and it works, with every knob below already defaulted. To change a
# color or a level, export the variable before sourcing this file.
#
# There is no log_base here and none is wanted: it exists in the C logger to print without decoration, which is
# what plain printf already is.
###################################################################################################################

# Not a knob: which release of CLibs this copy was cut from, for a consumer that needs to know what it's running.
export LOGGER_VERSION="0.1.0"

# `-` rather than `:-`: a color exported as an empty string is a request for no color, not for the default.
export COLOR_SEAGREEN="${COLOR_SEAGREEN-\x1b[38;2;0;182;182m}" # RGB color #00b6b6
export COLOR_DEFAULT="${COLOR_DEFAULT-\x1b[0m}"
export COLOR_WHITE="${COLOR_WHITE-\033[1;37m}"
export COLOR_GREEN="${COLOR_GREEN-\033[1;32m}"
export COLOR_YELLOW="${COLOR_YELLOW-\033[1;33m}"
export COLOR_RED="${COLOR_RED-\033[1;31m}"

# Every function below reads its arguments and its caller defensively (`${1-}`, `${BASH_SOURCE[1]-}`), so a
# script running under `set -u` is not aborted by its own logger -- with no message, or from an interactive
# shell where there is no calling file.

: "${LOGGER_MISC_ENABLED:=1}"
: "${LOGGER_TRACE_LEVEL:=50}"

log_info() {
	local fmt="${1-}"
	shift || true
	printf "${COLOR_GREEN}[INFO] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]-}" "${BASH_LINENO[0]}" "$@"
}

log_warn() {
	local fmt="${1-}"
	shift || true
	printf "${COLOR_YELLOW}[WARN] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]-}" "${BASH_LINENO[0]}" "$@"
}

log_fail() {
	local fmt="${1-}"
	shift || true
	printf "${COLOR_RED}[FAIL] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]-}" "${BASH_LINENO[0]}" "$@"
}

log_halt() {
	local fmt="${1-}"
	shift || true
	printf "${COLOR_RED}[HALT] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]-}" "${BASH_LINENO[0]}" "$@"
	exit 1
}

log_misc() {
	local level="${1:?log_misc: missing level}"
	shift

	if [[ "$LOGGER_MISC_ENABLED" -ne 0 && "$level" -le "$LOGGER_TRACE_LEVEL" ]]; then
		local fmt="${1-}"
		shift || true
		printf "${COLOR_WHITE}[MISC] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
			"${BASH_SOURCE[1]-}" "${BASH_LINENO[0]}" "$@"
	fi
}
