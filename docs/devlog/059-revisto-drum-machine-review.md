# Review Revisto Drum Machine

## Goal

Study the open-source [Revisto Drum Machine][repository] as a product and code
base, compare its musical model with the Calculator drum sequencer, and extract
useful evidence for the proposed preset electronic-drummer showcase and future
package boundaries.

The review inspected upstream `main` at commit
`40ddd0f0e13d2fba83e76e70f83e43e7dd1e7296` from 2026-09-25. No upstream or
local application code was changed.

## Product identity

Revisto Drum Machine is a GNOME desktop sample-pattern editor. It builds one
dynamically long sixteenth-note sequence, loops it through Pygame, saves and
loads it as MIDI, and renders it to several audio formats.

The Calculator is a small hardware performance sequencer. It keeps several
fixed patterns, arranges them in a chain, applies step intensity and swing, and
plays AMY drum voices directly.

| Concern | Revisto Drum Machine | Calculator drum sequencer |
| --- | --- | --- |
| Platform | Linux/GNOME desktop | M5Stack Core Gray hardware |
| Primary activity | Compose and export one sequence | Perform and arrange reusable patterns |
| Sound source | Replaceable audio samples | AMY synthesized drum voices |
| Drum parts | Dynamic | Four fixed tracks |
| Sequence geometry | Dynamic pages of 16 positions | Four patterns of 16 positions |
| Arrangement | No pattern chain | Twelve-position chain |
| Step value | On/off | Off, weak, normal, strong |
| Rhythmic grid | Fixed sixteenth notes | Selectable straight and triplet rates |
| Swing | None | 50--75% on straight rates |
| Persistence | Standard MIDI File | Not implemented |
| Export | MIDI plus rendered audio | Not implemented |
| Clock behavior | Worker thread with relative sleeps | Absolute deadlines and late-poll accounting |
| Main controls | Mouse, touch, and keyboard | Calculator keys and three Core buttons |

Both applications display a step grid, allow simultaneous drum triggers,
control BPM, show a playhead, and associate each row or track with a drum
sound. The shared surface resemblance hides significantly different domain
models.

## Feature set

The desktop application provides:

- ten bundled drum samples and six bundled MIDI patterns;
- custom WAV, MP3, Ogg, and FLAC samples;
- sample addition, replacement, removal, reordering, and preview;
- configurable MIDI note mapping for each drum part;
- dynamically added drum parts;
- sixteen-position pages, with more pages created as the sequence grows;
- BPM from 30 to 300 and master volume;
- start/pause, clear, and reset-to-default operations;
- MIDI save and load;
- offline WAV, FLAC, Ogg, and MP3 export;
- configurable pattern repetition during audio export;
- artist, title, and cover-art metadata;
- responsive eight- or sixteen-position page layouts;
- keyboard navigation, accessibility descriptions, and localization.

Its advertised "infinite pages" means there is no explicit application-level
page limit. Sparse Python dictionaries retain active positions and the UI adds
pages according to the highest active position. Host memory and practical UI
performance remain the physical limits.

## Musical data model

The central pattern state is approximately:

```text
drum part id
    -> sparse map of position index
        -> true
```

This is one dynamically sized boolean matrix. A later carousel page is another
segment of the same linear sequence, not an independently reusable pattern.
There is no domain object corresponding to the Calculator's `PatternBank` or
`PatternChain`.

The implementation calls grid positions "beats", but playback calculates each
position as:

```text
60 seconds / BPM / 4 subdivisions
```

The positions are sixteenth-note steps, with four positions per quarter-note
beat. The application has no explicit meter, bar, downbeat, grouping, rate, or
swing concept. For ecosystem vocabulary, `step`, `pulse`, or `subdivision`
would be less ambiguous than `beat` at this layer.

## Live timing

Playback runs in a Python worker thread:

```text
trigger the current position
    -> sleep for one sixteenth-note duration
    -> advance to the next position
```

This is easy to understand but offers weaker timing semantics than
`musical-clock`:

- processing time before `sleep()` contributes to the period and can
  accumulate drift;
- there is no absolute deadline that corrects a late iteration;
- a delay does not report the number of elapsed positions;
- a BPM edit changes a later sleep without explicitly preserving progress
  through the current interval;
- starting playback creates a local position at zero, so pause behaves as stop
  and restart rather than resumable transport;
- the loop cannot express unequal swing intervals without new policy;
- some GTK updates are scheduled with `GLib.idle_add`, while the playhead
  highlight is also invoked directly from the worker thread.

The Calculator instead separates musical interval policy, absolute deadline
accounting, logical catch-up, current-position state, trigger delivery, and
display updates. This remains the stronger foundation for the preset drummer.

## Live audio and offline rendering

Revisto has two audio paths:

```text
live:
DrumMachineService -> SoundService -> pygame.mixer.Sound

offline:
AudioExportService -> FFmpeg sample loader -> NumPy renderer
                   -> FFmpeg encoder
```

Live playback loads each part into memory and allows simultaneous sample
playback through 32 mixer channels.

The offline renderer creates a stereo floating-point buffer, places samples at
calculated sample offsets, mixes overlapping hits, repeats the pattern,
retains the final sample tail, normalizes the result, and passes it to FFmpeg
for encoding and metadata.

Separate live and offline mechanisms are reasonable because their operational
requirements differ. However, both independently interpret the musical grid
as four equal subdivisions per quarter note. Swing, alternate rates, or
microtiming would have to be implemented consistently in both paths. A shared
timed-trigger projection could reduce that risk:

```text
pattern + tempo policy
          |
          v
timed trigger sequence
       /       \
live scheduler  offline renderer
```

That projection could also feed MIDI export without making pattern storage a
MIDI byte stream.

## MIDI as the editable pattern file

Revisto uses Standard MIDI Files for its Open and Save Pattern operations, not
only for export. Saving:

- writes one track and one Set Tempo event;
- maps active cells to configured MIDI notes;
- places events on sixteenth-note boundaries;
- uses velocity 100 for every hit;
- gives every hit a sixteenth-note duration;
- represents simultaneous drums with zero-delta events.

Loading:

- reads the tempo;
- quantizes Note On positions to the nearest sixteenth;
- resolves known MIDI notes to current drum parts;
- creates a temporary silent part for an unknown MIDI note.

This works because nearly all persistent musical state can be reduced to:

```text
tempo + MIDI note at sixteenth-note position
```

The MIDI file does not preserve custom sample paths, custom part identities or
names, row order, replacement choices, master volume, an explicit length after
the final note, or editable velocity. `DrumPart` contains dictionary
serialization helpers, but the inspected application does not use them for a
lossless project file. An unknown imported note can recover its MIDI identity,
but not the custom sample that previously sounded it.

This is concrete evidence for the preceding project-format research. SMF can
act as a native pattern format while editable intent is essentially a
quantized MIDI performance. It cannot losslessly store the current
Calculator's pattern identity and reuse, chain, selected rate, explicit swing
value, AMY assignments, and three step intensities without conventions or
additional project data.

## Code organization

The source tree presents recognizable responsibilities:

```text
application and window
├── handlers
│   ├── file dialogs
│   ├── window actions
│   └── drag and drop
├── UI builders and helpers
├── services
│   ├── playback
│   ├── live sound
│   ├── MIDI pattern files
│   ├── offline audio rendering
│   └── audio encoding
├── models
│   └── DrumPart
└── interfaces
    ├── IPlayer
    └── ISoundService
```

### Effective choices

- File dialogs, sample management, offline rendering, and encoding are
  separate concerns.
- `DrumPart` gives a sound row an application identity distinct from its
  display name and MIDI note.
- The NumPy renderer is mostly independent of GTK.
- Input and output formats are catalogued explicitly.
- Unsupported imported MIDI notes remain visible rather than being discarded.
- Unsaved-change handling is explicit.
- Both MIDI output and audio rendering handle simultaneous drum hits.
- Responsive layout and accessibility are treated as product behavior rather
  than decoration.

### Limits relevant to our architecture

Several services accept the entire window and navigate back through it:

```text
PatternService -> window -> sound and playback services
AudioExportService -> window -> parts and sequence length
UIHelper -> window -> playback, sounds, carousel, and widgets
```

The directory boundaries therefore do not always produce dependency
boundaries. `DrumMachineService` also owns transport, thread lifecycle, tempo,
pattern state, length, sound triggering, playhead updates, carousel scrolling,
persistence delegation, and drum-part mutation.

The `IPlayer` and `ISoundService` interfaces provide some separation, but
window references still make most domain services difficult to construct in
isolation. An apparently obsolete `preset_service.py` also duplicates the
active `pattern_service.py` implementation, suggesting an incomplete rename.

No automated tests were found. Pattern MIDI conversion, offline rendering,
drum-part management, and a UI-independent playback policy would all be
natural host-test boundaries.

## Comparison of reusable evidence

| Capability | Revisto | Calculator | Boundary evidence |
| --- | --- | --- | --- |
| Boolean steps | Yes | Yes | Common lowest-level state, but insufficient alone |
| Simultaneous one-shots | Yes | Yes | Supports a reusable trigger/event projection |
| Step intensity | Constant velocity 100 | Three levels | A shared trigger must retain intensity |
| Sixteen positions | Page segment | Complete pattern | Equal geometry does not imply equal semantics |
| Pattern bank | No | Four patterns | Remains Calculator-specific evidence |
| Arrangement | No | Twelve positions | Revisto is not a second `PatternChain` consumer |
| Sound identity | Dynamic sample part | Curated AMY sound | Needs an adapter rather than a universal engine id |
| MIDI mapping | Configurable per part | MIDI-like AMY note currently identifies sounds | Promising interchange mapping |
| Swing and rates | No | Yes | These remain timing/product policy outside generic storage |
| Persistence | MIDI pattern | None | Useful reference with visible loss boundaries |
| Offline rendering | Yes | No | Possible future consumer of a timed-trigger projection |
| Deadline scheduling | Relative sleep | Absolute shared clock | `musical-clock` remains the reusable timing mechanism |

The review does not justify extracting `PatternBank` or `PatternChain`.
Revisto does strengthen the case for a smaller event boundary containing a
drum or sound identity plus intensity.

## Implications for the preset drummer

### Author presets as MIDI assets

Revisto ships its six default patterns as `.mid` files. The proposed rock,
Eurodance, jazz, and metronome grooves could likewise be authored and auditioned
with ordinary MIDI tools.

A general MIDI parser is unnecessary in the first embedded showcase. A better
pipeline is:

```text
authored .mid files
    -> host-side validation and conversion
    -> generated fixed-size C++ groove data
    -> firmware
```

This combines portable musical authoring with bounded deterministic runtime
storage. The source MIDI can remain visible beside the converter and generated
fixture so changes are reviewable.

### Preserve intensity in the trigger vocabulary

Revisto's constant velocity shows the limit of a boolean event model. The
Calculator already demonstrates that weak, normal, and strong hits materially
change the groove. A candidate shared one-shot event should therefore carry:

```text
drum or sound identity + intensity
```

The representation should not require the source to be a Calculator step or
the destination to be AMY.

### Keep presentation outside playback

The drummer's playback component should report musical results rather than
manipulate the display:

```text
elapsed pulses
current position
trigger events
bar or style boundary
```

The composition can independently send triggers to AMY and project the
position onto the screen. This retains the Calculator's stronger boundary
while implementing Revisto's approachable preset-oriented product.

## Result

Revisto Drum Machine is valuable prior art for sample workflow, MIDI pattern
interchange, audio export, responsive editing, and the distinction between a
human-facing drum part and its sound source. Its simpler single-sequence model
does not challenge the Calculator's pattern-bank or arrangement boundaries.

The most actionable idea is an authored-asset workflow: keep drummer presets
as standard MIDI sources, convert them on the host into fixed-size firmware
data, and let the embedded composition consume neutral trigger-and-intensity
events through the shared clocks and AMY adapter. This makes the new showcase a
useful composition experiment without turning MIDI parsing or a universal
sequencer model into premature package work.

## Sources

- [Revisto Drum Machine repository][repository]
- [Project README and feature list](https://github.com/Revisto/drum-machine#features)
- [Playback service](https://github.com/Revisto/drum-machine/blob/main/src/services/drum_machine_service.py)
- [MIDI pattern service](https://github.com/Revisto/drum-machine/blob/main/src/services/pattern_service.py)
- [Drum-part model](https://github.com/Revisto/drum-machine/blob/main/src/models/drum_part.py)
- [Drum-part manager](https://github.com/Revisto/drum-machine/blob/main/src/services/drum_part_manager.py)
- [Live sound service](https://github.com/Revisto/drum-machine/blob/main/src/services/sound_service.py)
- [Offline audio renderer](https://github.com/Revisto/drum-machine/blob/main/src/services/audio_renderer.py)
- [Application metadata and release history](https://github.com/Revisto/drum-machine/blob/main/data/io.github.revisto.drum-machine.metainfo.xml.in)

[repository]: https://github.com/Revisto/drum-machine
