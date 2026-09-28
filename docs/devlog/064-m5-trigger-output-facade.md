# 064 — Make the drummer's output a single composition element

## Goal

Remove AMY/M5 backend lifecycle details from the electronic drummer showcase.

## Result

The showcase now declares one `AmyM5TriggerOutput`. It configures musical lane
to MIDI-note mappings, passes that sink to `TriggerPatternCursor`, calls `begin()`
after board initialization, and services it with `update()`.

The showcase no longer knows about the speaker bridge, activity gate, synth
slot, AMY synth identifier, voice allocation, GM drum patch number, or default
speaker volume. `TriggerEventSink` remains visible as the useful composition
boundary between the pattern cursor and sound output.

## Validation

```text
pio test -d showcases/electronic-drummer -e native
just showcase-build 4
```

All six preset tests and the Core Gray firmware build pass. Hardware playback
is still required before the package candidates are committed and published.
