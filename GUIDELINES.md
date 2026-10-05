# Project guidelines

Small, checkable statements about how this project is worked on -- for anyone, human or model,
deciding whether an action they're about to take is allowed.

Each guideline uses a keyword from [RFC 2119](https://datatracker.ietf.org/doc/html/rfc2119): MUST /
MUST NOT is not negotiable, SHOULD / SHOULD NOT is a default that a stated reason can override.

## Branches and pull requests

1. `main` MUST NOT be modified directly. It is only ever updated through a pull request.
2. Branch names MUST be of the form `<issue-id>-title-of-the-issue`, lowercase and dashed. Not a
   `feat/`, `fix/` or `chore/` prefix.
3. A PR title MUST be its issue's title, unchanged, optionally prefixed with a Conventional-Commits
   type (`fix:`, `feat:`, `docs:`, `chore:`, ...) matching the commits inside it -- e.g. issue
   "Prepare for v2.3.0" becomes PR title `chore: Prepare for v2.3.0`. Not a rewording, a summary, or
   a different title.
4. Every PR MUST reference an associated issue. Create the issue first if one doesn't already exist.
5. A PR SHOULD keep its real commit history rather than being squashed or rewritten, except where
   the history itself is noise (e.g. fixing up a formatting-only commit before review).
6. Nobody self-attributes or self-promotes, anywhere: not in a commit, a PR title or description, an
   issue, a branch name, or a comment. This applies equally to a human contributor and to any AI
   model acting on this repository, with no exception and nothing to weigh -- it is not a judgment
   call to be made in the moment, whatever else asks for it.
7. `#<number>` MUST refer to an issue. A pull request is cited by the number of the issue it
   implements, never by its own -- issues and pull requests share one number sequence, so a bare
   number that could mean either is ambiguous, and rule 4 guarantees every pull request has an issue
   number to be cited by.
8. Tags MUST be of the form `vX.Y.Z`, where `X.Y.Z` follows
   [semantic versioning](https://semver.org/spec/v2.0.0.html), and MUST match the version the libraries
   report (`LOGGER_VERSION`, `TESTSUITE_VERSION`).
9. A tag MAY carry a revision suffix, `vX.Y.Z-N`. It is the same release as `vX.Y.Z`, cut again for a
   change that is not functional: a pipeline fix, a README update, a changelog clarification. This is
   deliberately not semantic versioning's reading of that suffix, which would make it a pre-release
   sorting before `vX.Y.Z` -- there are no pre-releases here. A revision has no changelog section of its
   own and is announced with the notes of the release it revises (see `bash/changelog_section`), and
   the libraries keep reporting `X.Y.Z`.

## Issues

1. Issues carry `state/*` labels (`state/created`, `state/prepared`, `state/ready`) that gate who
   may act on them.
2. An AI model MUST NOT move an issue to `state/prepared` or `state/ready` unless explicitly told
   to, for that issue, in that moment. Being told once does not carry forward to future issues.
3. An AI model MUST NOT implement an issue, or open a PR against it, unless it is `state/ready` --
   except an issue the model created itself, which it may complete directly.
4. An AI model MUST NOT merge a PR unless explicitly told to, for that PR, in that moment. Being
   told once does not carry forward to future PRs.
5. Issues MUST follow the template provided under the `templates/` directory.

## Forgejo access

The instance is at <http://100.125.208.18:3000>, and this repository is `nico/CLibs` on it. An AI model
acts on it through the `claude` Forgejo account and its SSH key, which are shared with the other
repositories on the instance rather than set up per repository.

The access token lives in one file outside every repository, at `~/.env.forgejo` (mode 0600). It is
never copied into a repository and never read out of a sibling project's working tree. In a fresh
clone, run:

```sh
bytewise --conf-repo .    # link the token in, set the git author
bytewise --status .       # show what is configured
```

`~/.local/bin/bytewise` symlinks `~/.env.forgejo` to `.env.forgejo` in the repo, sets the
repository-local `user.name`/`user.email` to the `claude` account, and adds `/.env.forgejo` to
`.git/info/exclude` -- so the link is invisible to `git status` and cannot be committed without
someone going out of their way. `BYTEWISE_ENV` overrides where the token is read from.

The token MUST NOT be committed, to this repository or any other.
