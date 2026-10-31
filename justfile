libs := "logger bash-logger testing"

# List available recipes.
default:
    @just --list

# Compiles and runs each library's minimal example, as a smoke test.
test:
    #!/usr/bin/env bash
    set -euo pipefail

    work="$(mktemp -d)"
    trap 'rm -rf "$work"' EXIT

    cc -std=c11 -Wall -Wextra -Werror -Ilogger logger/examples/minimal.c -o "$work/logger" >&2
    "$work/logger" >/dev/null 2>&1
    echo "ok   logger"
    logger/tests/run >/dev/null
    echo "ok   logger (output checks)"

    bash bash-logger/examples/minimal.sh >/dev/null 2>&1
    echo "ok   bash-logger"
    bash-logger/tests/run >/dev/null
    echo "ok   bash-logger (output checks)"

    make -C testing EXAMPLE=minimal run >/dev/null
    echo "ok   testing"

    make -C testing EXAMPLE=production run >/dev/null
    echo "ok   testing (production example)"

    testing/tests/run >/dev/null
    echo "ok   testing (runner checks)"

    make -C testing clean >/dev/null

# Packages one library into a version-named tarball under `out` (dist/ by default).
package lib version out="dist":
    #!/usr/bin/env bash
    set -euo pipefail
    mkdir -p "{{out}}"
    tar --transform "s,^{{lib}},{{lib}}-{{version}}," -czf "{{out}}/{{lib}}-{{version}}.tar.gz" "{{lib}}"

# Packages every library at once.
package_all version out="dist":
    #!/usr/bin/env bash
    set -euo pipefail
    for lib in {{libs}}; do
      just package "$lib" "{{version}}" "{{out}}"
      echo "wrote {{out}}/$lib-{{version}}.tar.gz"
    done

# Checks that packaging produces what it should, without a tag involved.
#
# No tag involved is the point: the alternative is pushing a tag to find out whether the release job
# works, and a tag is the one thing that cannot be taken back quietly.
test_package:
    #!/usr/bin/env bash
    set -euo pipefail

    work="$(mktemp -d)"
    trap 'rm -rf "$work"' EXIT

    just package_all v9.9.9 "$work" >/dev/null

    for lib in {{libs}}; do
      archive="$work/$lib-v9.9.9.tar.gz"
      [[ -s "$archive" ]] || { echo "missing $archive"; exit 1; }

      listing="$(tar -tzf "$archive")"
      echo "$listing" | grep -qx "$lib-v9.9.9/README.md" \
        || { echo "$archive is missing its README"; exit 1; }
      echo "$listing" | grep -q "$lib-v9.9.9/examples/" \
        || { echo "$archive is missing its examples/"; exit 1; }

      echo "ok   $lib-v9.9.9.tar.gz"
    done

# Lints every shell file this repository ships or releases with.
lint:
    shellcheck -x bash-logger/logger.bashrc bash-logger/examples/minimal.sh \
      bash/cap_output bash/changelog_section bash/check_versions bash/tests/run \
      testing/gen_test_registry logger/tests/run bash-logger/tests/run testing/tests/run

# Checks the tooling in bash/ against copies it is free to break.
test_tooling:
    bash/tests/run

# Checks that a tag names the version every library reports, as the release job does before publishing.
check_versions tag:
    bash/check_versions "{{tag}}"
