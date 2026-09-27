# 060 — Compose an electronic drummer showcase

## Goal

Create a second consumer for the Calculator's extracted `step-trigger` package
that is primarily composition: fixed musical material, timing, an audio route,
and three device buttons. The showcase should also demonstrate why trigger
patterns are not intrinsically sixteen-position drum patterns.

## Design

`showcases/electronic-drummer` combines three existing boundaries:

- `step-trigger` stores four fixed-capacity lanes and emits levelled triggers;
- `musical-clock` advances an absolute, drift-resistant deadline;
- `amy-synth-m5` renders mapped GM drum notes through the Core Gray speaker.

Only preset authorship and application wiring remain local. Rock and Euro Pop
use sixteen sixteenth-note positions, Jazz uses twelve triplet positions, and
Metronome uses four quarter-note positions. All represent one four-beat cycle,
so the same BPM has the same musical meaning even though physical deadline
intervals differ.

Button A cycles styles. Buttons B and C adjust tempo in 5 BPM increments.
Style changes restart on a downbeat; tempo changes preserve phase. Late polls
advance logical position but do not burst-play missed drum events.

This first slice uses the sibling `step-trigger` directory so it can be tested
before its first registry publication. A following release-integration slice
will replace that path with `fcz2/step-trigger@0.1.0`, which also makes Showcase
4 independently buildable in CI.

## Validation

```text
pio test -d showcases/electronic-drummer -e native
pio run -d showcases/electronic-drummer
just showcase-list
```

Hardware validation proceeded iteratively on an M5Stack Core Gray: the style
and BPM controls worked, and the listening passes below selected the final Jazz
and Bossa renderings.

The first listening pass found that Ride Cymbal 1 dominated the other Jazz
voices on the Core Gray speaker. The next candidate replaces the repeating ride
lane with pedal hi-hat and removes the now-duplicate pedal-hi-hat hits on beats
2 and 4. Kick and snare are unchanged.

The pedal hi-hat candidate was still unpleasantly high-pitched on the device.
The next listening candidate uses GM Side Stick (note 37) for the repeating
pulse. This also tests a sound that is not in the Calculator's current 20-sound
palette and may inform a later kit revision if it works well.

Side Stick was better, but the improvement could have come mainly from removing
the previous high-pitched sound. For a controlled comparison, the temporary
style cycle now contains four otherwise-identical Jazz variants: Side Stick
(37), Ride Cymbal 2 (59), Ride Bell (53), and Low Tom (45). The selected winner
can later replace all four variants.

Ride Cymbal 2 sounded the most recognizably jazzy, but its sample tail was still
too long. The next comparison retains Side Stick as Jazz 1 and the natural Ride
Cymbal 2 as Jazz 2, then changes Jazz 3 and 4 to the same ride with 220 ms and
120 ms note-off gates. Besides choosing an envelope by ear, this determines
whether AMY patch 258 honors note-off for its PCM cymbal voices.

The 220 ms and 120 ms gates produced little audible difference, indicating that
the GM PCM cymbal behaves effectively as a one-shot in this use. The gate state
and note-off scheduling were removed rather than retained as ineffective app
complexity. The next controlled comparison uses Side Stick (37), Electric
Snare (40), Claves (75), and Maracas (70). Low Tom was rejected before testing
because it is musically wrong as the Jazz groove's core timekeeping voice.

The controlled non-cymbal comparison selected Maracas. On the Core Gray it
retained a clear triplet pulse without the long, dominant cymbal tail and felt
better than Side Stick, Electric Snare, or Claves. The temporary Jazz variants
were therefore collapsed back to one `JAZZ` style using GM note 70.

An `OFF` preset was added as the fifth style. It uses an empty four-position
quarter-note cycle, keeping transport behavior uniform while providing a simple
way to silence the drummer with the existing style button.

A Bossa preset adds another musical use for GM Side Stick (37): here it plays
a sparse cross-stick figure rather than carrying every Jazz subdivision. The
16-position pattern combines kick on beat 1, the and of 2, beat 3, and the and
of 4; a five-hit cross-stick figure; alternating maracas eighths; and weak pedal
hi-hat on beats 2 and 4. This avoids the Calculator's harsh Claves sound while
testing whether Side Stick belongs in a future revision of its sound palette.

Compared with the Zoom G1 Four `Bossa1` rhythm at the same BPM, the first Bossa
candidate felt busier and less easy-going. Two controlled alternatives were
added: `BOSSA SOFT` retains a 16-position bar but makes the timekeeping voices
weak and removes pedal hi-hat; `BOSSA 2BAR` uses the full 32-position package
capacity and spreads only five cross-stick hits across two bars. The kick idea,
maracas subdivision, sounds, and BPM remain comparable.

The first 32-position candidate still felt filled because it duplicated the
kick and maracas density across both bars. It was thinned to four kick hits,
four cross-sticks, and eight weak quarter-note maracas across the entire
two-bar phrase—roughly one original bar's event count spread over twice the
time.

Neither the softened one-bar version nor the sparse two-bar version achieved
the calm, easy-going feel of the Zoom G1 Four `Bossa1` reference. All Bossa
presets were removed from the product-facing style cycle. The failed variants
remain documented here as evidence that lower event density and longer pattern
length alone did not reproduce the desired feel.

A locally supplied Standard MIDI File offered a concrete replacement reference:
`~/Downloads/bossa/BOSSA.MID`, format 1 with 192 PPQ, 4/4 meter, and an authored
tempo of 110 BPM. Its first full groove begins after 768 ticks and uses Acoustic
Bass Drum (35), Side Stick (37), Closed Hi-Hat (42), Cabasa (69), and Maracas
(70) with small humanizing offsets. The showcase's four-lane boundary cannot
preserve both shaker sounds, so `BOSSA MIDI` quantizes the first two groove bars
to a 32-position sixteenth grid and merges Cabasa/Maracas into alternating
normal/weak Maracas hits. This is explicitly an approximation of source events,
not another style invented from a textual recipe.

The first sound substitution test changes only the source Side Stick lane from
GM note 37 to Open Cuica (79), a sound already present in the Calculator palette.
This tests whether its short body works better as the perceived rim-click on the
Core Gray while preserving the imported rhythm exactly.

The Open Cuica substitution was passable in the hardware test and became the
selected Bossa rendering. The temporary `BOSSA MIDI` label was shortened to
`BOSSA`; its 32-position source-derived pattern remains intact.

## Limits

The showcase has no pause control, swing setting, arrangement, persistence, or
pattern editor. Those are intentionally outside this composition slice. The
presets are starting musical sketches and should be tuned by ear on hardware.
