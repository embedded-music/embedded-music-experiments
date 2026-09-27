# 061 — Publish the trigger boundary and remove sibling checkout plumbing

## Goal

Turn the electronic drummer into a genuinely standalone second consumer of
the Calculator's extracted trigger package.

## Release path

The first `step-trigger-v0.1.0` workflow attempt stopped before publication
because Calculator CI still depended on the sibling `musical-clock` checkout.
That failure was useful boundary evidence rather than a package failure.

The validated alternating-clock snapshot was published as
`fcz2/musical-clock@0.2.0`. Calculator moved to that registry dependency, passed
all 16 native tests and its Core Gray firmware build, and was pushed as commit
`a643fe7c`. The unpublished trigger tag was then moved to that fix.

The corrected trigger workflow passed package tests, Calculator tests, package
packing, an isolated archive consumer, and the Calculator firmware build before
publishing `fcz2/step-trigger@0.1.0`.

## Umbrella integration

Showcase 4 now declares `fcz2/step-trigger@0.1.0` instead of a relative sibling
path. This removes workspace-layout knowledge from the composition and allows
the umbrella CI matrix to build the electronic drummer from a clean checkout.

## Validation

```text
pio test -d showcases/electronic-drummer -e native
just showcase-build 4
```

The next plumbing-reduction candidate is runtime composition: both Calculator
and the electronic drummer currently translate trigger levels, map lanes to GM
notes, wake AMY audio, and call `AmySynthSlot::noteOn`. That repeated adapter
belongs near the AMY backend, with per-sound velocity calibration kept separate
from the semantic `StepLevel` values.
