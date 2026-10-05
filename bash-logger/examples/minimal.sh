#!/usr/bin/env bash
# Bare-minimum usage of logger.bashrc. Run with:
#
#   ./minimal.sh
set -euo pipefail

# shellcheck source=bash-logger/logger.bashrc
source "$(dirname "${BASH_SOURCE[0]}")/../logger.bashrc"

log_info "starting up"
log_warn "this is a warning about %s" "something"
log_fail "this is what a failure looks like"
log_misc 10 "a trace message at level %d" 10
