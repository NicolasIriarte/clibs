# Description

Cuts vX.Y.Z. The scope is whatever is merged to `main` and unreleased at the point this is filed -- see
`CHANGELOG.md`'s `[Unreleased]` section for what that is.

# Test

- [ ] `just test_package` passes, so the tag publishes what it should rather than an empty page.
- [ ] `CHANGELOG.md` has something under `[Unreleased]` before this closes it out -- a tag with nothing
      to say fails the release job rather than publishing a page with no notes.
- [ ] `just metrics vX.Y.Z` records this version's figures.
- [ ] CI is green on the commit being tagged, hardware included -- a tag is never skipped for it.

# Tasks

- [ ] Rename `[Unreleased]` to `[X.Y.Z] - YYYY-MM-DD`, and open a fresh `[Unreleased]` above it.
- [ ] Run `just metrics vX.Y.Z` and commit the row it adds.
- [ ] Merge the PR, tag the merge commit `vX.Y.Z`, and push the tag.
