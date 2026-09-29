# 066 — Consume released drummer dependencies

## Goal

Keep the electronic drummer independently buildable without reconstructing the
author's sibling-repository workspace in CI.

## Release boundary

The reusable trigger player was released as `fcz2/step-trigger@0.2.0`. The M5
trigger output facade that consumes it was then released as
`fcz2/amy-synth-m5@0.3.3`. Publishing from the bottom of the dependency graph
ensures each package can resolve its released dependencies on its own.

The electronic drummer now pins both registry releases in its firmware build.
Its native tests pin the same `step-trigger` release. No showcase dependency
refers to a sibling checkout, and the umbrella CI needs no special multi-repo
checkout logic.

This deliberately favors frequent package snapshots over keeping compatible
but unreleased work coupled across repositories. Pre-1.0 breakage is handled
by releasing a new version and updating consumers together.

## Validation

After clearing the showcase's resolved dependency directory, run:

```text
pio test -d showcases/electronic-drummer -e native
pio run -d showcases/electronic-drummer
```

The isolated GitHub Actions Showcase 4 job is the final proof that neither
build relies on a sibling repository. Hardware sound and control checks remain
pending.
