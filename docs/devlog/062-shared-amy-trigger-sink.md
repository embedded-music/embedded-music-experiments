# 062 — Compose the electronic drummer with the AMY trigger sink

## Goal

Make the electronic drummer showcase more visibly a composition of reusable
modules and remove its local audio-output adapter.

## Composition

The showcase now configures `AmyTriggerSink` from each drummer preset and sends
the shared trigger sequencer's events directly into it. Style presets continue
to own patterns and MIDI-note choices; the AMY package owns audio wake-up,
accent velocity conversion, and note output.

An Off preset disables its zero-valued lanes explicitly, avoiding an accidental
MIDI note 0 if a pattern is later added to that preset.

## Validation

```text
pio test -d showcases/electronic-drummer -e native
just showcase-build 4
```

The hardware acceptance target is unchanged playback for Off, Rock, Jazz, and
Bossa, including distinct weak, normal, and strong accents.

The same shared output path passed its hardware playback check in the
Calculator before publication.

The final showcase build resolves `fcz2/amy-synth-m5@0.3.2` from the registry;
the composition no longer depends on a sibling AMY repository path.
