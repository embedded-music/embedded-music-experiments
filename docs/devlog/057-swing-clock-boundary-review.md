# Review the sequencer boundaries after swing

## Goal

Use the Calculator drum sequencer's progression from demo to small instrument
to refine ecosystem vocabulary and promote one boundary supported by running
code, without extracting its product-specific sequencer model.

## Current evidence

The Calculator now validates on hardware:

- four tracks, four patterns, and a twelve-position pattern chain;
- sixteen fixed positions per pattern and boundary-quantized pattern changes;
- per-track sound selection and three authored step levels;
- explicit weak, normal, and strong one-shot velocities;
- tempo, musical step division, and 50--75% percentage swing;
- equal triplet divisions that bypass pairwise swing;
- phase-preserving live timing changes;
- logical late-poll catch-up without stale audio bursts;
- modal Calculator input and reusable held Core-button modifiers.

This evidence supersedes the old single-pattern, boolean-step description in
the radar. It also exposes a boundary that the original constant-period clock
could not represent.

## Extracted boundary

`musical-clock` now owns `AlternatingDeadlineClock`: a platform-independent
absolute-deadline accumulator for a repeating pair of physical intervals. It
reports all elapsed intervals after a late poll and preserves progress through
the current interval when the pair changes.

The package deliberately does not know why intervals alternate. The Calculator
adapter owns this translation:

```text
tempo + step division -> straight interval
swing percentage      -> long/short interval pair
triplet policy         -> equal interval pair
```

This vocabulary separates three layers:

```text
musical settings -> interval schedule -> absolute deadline accounting
```

The extraction is justified by a real boundary failure: the shared periodic
clock served both metronome and sequencer, but could not express the
hardware-validated swing schedule. The new mechanism is useful without
claiming a common sequencer, transport, or groove model.

## Boundaries kept local

| Local concept | Reason not to extract now |
| --- | --- |
| `SequencerSettings` and `StepRate` | BPM/division/swing vocabulary is promising, but triplet bypass and available divisions are still product policy. |
| `StepLevel` | Weak/normal/strong is validated for this drum editor, but other instruments may need MIDI velocity, probability, gate, or continuous values. |
| `SequencerEvent` / sink | It currently carries a curated drum sound id and local step level; a second one-shot consumer should determine any shared trigger contract. |
| `PatternBank` | Four tracks, four slots, sound ownership, and fixed sixteen-step geometry are Calculator composition choices. |
| `PatternChain` | Twelve visible positions and the non-empty-chain invariant come directly from this UI. |
| `HeldButtonGesture` | Reusable press/consume/release state is a good local module, but it is hardware interaction policy rather than a music-domain package. |
| command map, router, and views | They translate this Calculator surface into this instrument's modes and remain application code. |

## Organization result

The Calculator keeps a thin `SequencerStepClock` adapter. It derives a physical
interval pair and delegates deadline accumulation to `musical-clock`. The
obsolete constant-clock adapter was removed, and the native test environment
now declares the same package dependency as firmware.

This is the desired package direction for the ecosystem: promote small
mechanisms with explicit ownership, while allowing composition vocabulary to
remain local until another instrument challenges it.

## Verification

```text
cd ../musical-clock && just test
cd ../calculator-face-input && just test-native
cd ../calculator-face-input && just build-sequencer
```

The clock package's eleven tests, the Calculator's sixteen tests, and the Core
Gray firmware build passed.
