# 065 — Hide pattern playback plumbing

## Goal

Keep the electronic drummer showcase focused on musical composition rather
than coordinating a clock and playhead directly.

## Composition boundary

The shared `TriggerPatternPlayer` composes an alternating deadline clock,
pattern cursor, current-pattern source, and trigger-event sink. The showcase
now supplies only its preset source, AMY trigger output, and straight step
duration. Style changes restart playback; tempo changes preserve phase.
Startup and style changes explicitly request `EmitImmediately`, so the first
step sounds without a separate emission call while silent synchronization
remains available to other consumers.
The showcase passes `StepIntervals::constant(stepIntervalUs())`, explicitly
stating that every step uses the same interval without exposing the underlying
alternating clock.

The same player was applied to the standalone metronome, where level maps to
accent versus regular click. That second shape demonstrates that the boundary
is about timed trigger patterns, not drum kits or AMY.

## Validation

```text
pio test -e native
pio run
```

The electronic drummer's six native tests and firmware build pass. The
metronome firmware also builds with the same local package candidate. Hardware
sound and control checks remain before committing the slice.
