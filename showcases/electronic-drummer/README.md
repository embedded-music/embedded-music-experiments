# Electronic Drummer Showcase

This firmware is a small composition of `step-trigger`, `musical-clock`, and
the AMY M5 audio backend. It plays five fixed musical presets plus an Off mode
rather than exposing a pattern editor:

- Rock: a 16-position sixteenth-note grid.
- Jazz: a 12-position triplet grid with a maracas pulse.
- Bossa: a 32-position adaptation of the opening full groove in the
  locally supplied `BOSSA.MID`, intended for listening at its authored 110 BPM.
- Euro Pop: a 16-position four-on-the-floor grid.
- Metronome: a 4-position quarter-note grid with a downbeat accent.
- Off: an empty 4-position cycle that silences the drummer.

The different grids intentionally exercise `TriggerPattern`'s variable active
length. Each preset maps the same four abstract trigger lanes to its own GM
drum notes.

Maracas provide the repeating Jazz timekeeping voice. This was selected by
listening on the Core Gray after controlled comparisons with ride cymbals,
pedal hi-hat, side stick, electric snare, and claves.

`BOSSA.MID` uses separate cabasa and maracas parts, slight timing offsets, and
more than four percussion voices. This showcase quantizes the first two full
groove bars to sixteenths and merges cabasa/maracas into one alternating-level
Maracas lane alongside Acoustic Bass Drum, Open Cuica, and Closed Hi-Hat. Open
Cuica substitutes for the source Side Stick in the rim-click role; this was the
preferred rendering in the Core Gray hardware listening test.


## Controls

- Button A: select the next style.
- Button B: decrease tempo by 5 BPM.
- Button C: increase tempo by 5 BPM.

Tempo is constrained to 40–240 BPM. Changing style restarts at the downbeat;
changing tempo preserves the current step's phase.

## Validation

```bash
pio test -d showcases/electronic-drummer -e native
pio run -d showcases/electronic-drummer
```

On an M5Stack Core Gray, verify that the musical styles are recognizably
distinct, Jazz keeps the same BPM while using its triplet grid, Bossa feels
natural at 110 BPM, Off is silent, and tempo changes do not restart the bar.

The initial local dependency on the sibling `step-trigger` package is
intentional. Replace it with the PlatformIO registry release after publishing
`step-trigger-v0.1.0`; that replacement is the release-integration slice.
