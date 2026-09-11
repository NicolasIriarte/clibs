#!/bin/bash

###################################################################################################################
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

export COLOR_SEAGREEN="${COLOR_SEAGREEN:-\x1b[38;2;0;182;182m}" # RGB color #00b6b6
export COLOR_DEFAULT="${COLOR_DEFAULT:-\x1b[0m}"
export COLOR_WHITE="${COLOR_WHITE:-\033[1;37m}"
export COLOR_GREEN="${COLOR_GREEN:-\033[1;32m}"
export COLOR_YELLOW="${COLOR_YELLOW:-\033[1;33m}"
export COLOR_RED="${COLOR_RED:-\033[1;31m}"

: "${LOGGER_MISC_ENABLED:=1}"
: "${LOGGER_TRACE_LEVEL:=50}"

log_info() {
	local fmt="$1"
	shift
	printf "${COLOR_GREEN}[INFO] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]}" "${BASH_LINENO[0]}" "$@"
}

log_warn() {
	local fmt="$1"
	shift
	printf "${COLOR_YELLOW}[WARN] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]}" "${BASH_LINENO[0]}" "$@"
}

log_fail() {
	local fmt="$1"
	shift
	printf "${COLOR_RED}[FAIL] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]}" "${BASH_LINENO[0]}" "$@"
}

log_halt() {
	local fmt="$1"
	shift
	printf "${COLOR_RED}[HALT] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
		"${BASH_SOURCE[1]}" "${BASH_LINENO[0]}" "$@"
	exit 1
}

log_misc() {
	local level="$1"
	shift

	if [[ "$LOGGER_MISC_ENABLED" -ne 0 && "$level" -le "$LOGGER_TRACE_LEVEL" ]]; then
		local fmt="$1"
		shift
		printf "${COLOR_WHITE}[MISC] ${COLOR_SEAGREEN}[%s:%d]${COLOR_DEFAULT} - ${fmt}\n" \
			"${BASH_SOURCE[1]}" "${BASH_LINENO[0]}" "$@"
	fi
}
