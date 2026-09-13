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
    "$work/logger" >/dev/null
    echo "ok   logger"

    bash bash-logger/examples/minimal.sh >/dev/null
    echo "ok   bash-logger"

    testing/gen_test_registry testing/examples/minimal "$work/testsuite_registry.h"
    cc -std=c11 -Wall -Wextra -Werror -I"$work" -Itesting -Ilogger \
      testing/examples/minimal/main.c testing/examples/minimal/1_addition.c testing/testsuite.c \
      -o "$work/testing" >&2
    "$work/testing" >/dev/null
    echo "ok   testing"

    testing/gen_test_registry testing/examples/production "$work/testsuite_registry.h"
    cc -std=c11 -Wall -Wextra -Werror -I"$work" -Itesting -Ilogger \
      testing/examples/production/main.c testing/examples/production/1_stubs.c testing/testsuite.c \
      -o "$work/testing-production" >&2
    "$work/testing-production" >/dev/null
    echo "ok   testing (production example)"

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
