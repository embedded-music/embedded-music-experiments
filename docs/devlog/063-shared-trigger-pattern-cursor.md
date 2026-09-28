# 063 — Compose the drummer with the shared pattern cursor

## Goal

Replace the electronic drummer's manual step arithmetic and late-step
branch with the cursor extracted from the Calculator.

## Composition

The deadline clock continues to report elapsed intervals.
`TriggerPatternCursor` now advances and wraps the playhead for each preset's
actual pattern length, decides whether an on-time step should sound, and emits
that step into `AmyTriggerSink`.

Changing style resets the cursor before the new preset's first step is
played. Tempo scheduling, buttons, display, preset selection, and diagnostic
logging remain showcase code.

## Validation

```text
pio test -d showcases/electronic-drummer -e native
just showcase-build 4
```

All six preset tests and the Core Gray firmware build pass with the local
`step-trigger@0.2.0` and `amy-synth-m5@0.3.3` candidates.

Preset declarations now use shared `TriggerPatternCell` arrays and
`TriggerPatternLoader`; the showcase-local `AuthoredStep` translation has been
removed. The same loader is suitable for a future saved-project decoder.
