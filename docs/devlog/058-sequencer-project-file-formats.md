# Research sequencer project file formats

## Goal

Decide whether the Calculator drum sequencer and a future preset electronic
drummer should use an existing music-file standard as their native project
model, and identify which external formats are useful references or future
interchange targets.

The distinction matters because a rendered performance and an editable project
do not preserve the same information. A performance needs timed events. A
project must also preserve reusable patterns, their arrangement, sound
assignments, musical settings, and product-specific editing intent.

## Format comparison

| Format family | Patterns and arrangement | Sounds | Adoption | Fit for this ecosystem |
| --- | --- | --- | --- | --- |
| Standard MIDI File 1.0 | A finite timestamped timeline; pattern structure is normally flattened | MIDI program and percussion references, not engine state | Very broad | Performance export |
| Tracker modules (`MOD`, `XM`, `IT`) | Reusable row patterns plus an order list | Instruments and often samples | Established mainly among trackers | Strong structural prior art |
| DAWproject | Clips, scenes, tracks, arrangement, notes, automation, audio, and devices | Audio and device descriptions | Growing among desktop DAWs | Open interchange reference, too broad for firmware |
| MusicXML | Measures, notation, and score structure | Notated instruments | Broad among notation programs | Poor fit for a groovebox project |
| MIDI 2.0 Clip File | One sequence of Universal MIDI Packet events | MIDI 2.0 performance data and profile-related information | Emerging | Future performance interchange |
| Manufacturer project formats | Exactly the structures implemented by one product family | Usually complete within that ecosystem | Usually limited to that vendor or device family | Normal precedent for lossless native saving |
| MIDI SysEx dumps | Manufacturer-defined payload inside a standard transport envelope | Whatever the manufacturer defines | Broad backup mechanism, not a common project schema | Possible transfer mechanism, not a data model |

## Standard MIDI Files preserve performance

SMF is a compact interchange format for timestamped MIDI data. It supports
tempo, time signature, tracks, names, and multiple simultaneous events, but it
does not standardize the Calculator's editable pattern bank or chain.

A future Calculator Format 1 export could contain:

```text
track 0: tempo, meter, section markers
track 1: Calculator drum track 1
track 2: Calculator drum track 2
track 3: Calculator drum track 3
track 4: Calculator drum track 4
```

Step positions would become MIDI ticks, weak/normal/strong levels would become
velocities, and the selected AMY drums would map to the nearest General MIDI
percussion notes. A chain would be expanded into a finite timeline.

Swing illustrates the loss of editing intent. At 480 ticks per quarter note,
the second eighth begins at tick 240 for straight timing, tick 288 for 60%
swing, tick 320 for triplet-like 66.7% swing, and tick 360 for 75% swing. SMF
can reproduce those positions, but a reader cannot reliably determine whether
they came from a swing control or individually moved events.

SMF therefore makes a good export target for DAWs and other instruments, but a
poor sole project format. Pattern reuse, chain entries, AMY identities, the
chosen swing value, and application invariants would be flattened or
approximated.

The MIDI Association explicitly describes SMF as an interchange format and
notes that its compact disk representation may be unsuitable for a sequencer's
quickly accessed in-memory representation.

## Tracker modules preserve composition structure

Tracker module formats are the closest established precedent for the current
Calculator model:

```text
tracker module                 Calculator project
--------------                 ------------------
instruments and samples        track sound assignments
row/channel patterns           step/track patterns
order list                     pattern chain
```

Unlike SMF, the order list can reference the same pattern repeatedly without
copying its events. This separation between sound assignments, reusable
patterns, and ordered arrangement is useful ecosystem vocabulary.

The formats are not neutral, however. Rows, channels, effect commands, sample
playback, and tempo conventions reflect tracker workflows. Calculator rate,
swing, three intensity levels, and AMY sound selection would still require
translation rules. Polyend's Tracker history provides practical evidence:
MOD/IT-related interchange was removed, and Polyend discussion describes
feature-parity and conversion limitations between its project model and
traditional tracker formats.

Tracker modules should guide the project hierarchy, not become the native
Calculator representation.

## DAWproject is open but deliberately DAW-sized

DAWproject is an open ZIP/XML exchange format developed by Bitwig and PreSonus.
Its model includes transport, tracks, channels, clips, scenes, notes,
automation, audio, devices, and arrangement state. It also allows an importer
to preserve available structure or flatten structures it cannot represent.

This is useful evidence that even a broad open format distinguishes native
application storage from interchange. DAWproject explicitly lists being a
DAW's native file format as a non-goal.

For these microcontroller compositions, ZIP/XML processing and DAW-level
device concepts would add complexity without improving the on-device model. A
future desktop tool could translate a compact ecosystem project into
DAWproject, but firmware should not depend on it.

## MIDI 2.0 does not yet provide a groovebox project model

The MIDI 2.0 Clip File stores one sequence of Universal MIDI Packet messages,
playing a role similar to an SMF 1.0 Format 0 file while retaining MIDI 2.0
resolution and expression. It remains a clip/performance interchange format,
not a standard pattern-bank and chain model.

The MIDI Association has also described a larger container intended to carry
multiple tracks, clips, and media. As of its February 2026 status report, that
container remained forthcoming. It may become a useful export target, but it
does not currently justify basing embedded persistence on MIDI 2.0.

## Hardware practice is mostly proprietary

Hardware grooveboxes commonly combine a lossless native project with simpler
interchange formats:

```text
native proprietary project
    + WAV or sample interchange
    + MIDI input/output or export
    + SD, USB, companion-app, or SysEx backup
```

Examples show that the standard part is usually the transport or media, not the
complete project schema:

- Elektron Digitakt can dump projects, patterns, and Sounds over SysEx, while
  sample transfer remains separate. The SysEx payload expresses Elektron's
  model.
- Roland MC-101 saves project and audio data to SD card and supplies
  Roland-specific clip packages and clip files alongside MIDI connectivity.
- Ableton Move transfers Move Sets into the Ableton Note and Live ecosystem,
  providing ecosystem round trips rather than universal groovebox projects.

This fragmentation is not merely vendor preference. Products disagree about
parameter locks, probability, conditions, sample slicing, scenes, clip
launching, synth patches, automation, and pattern hierarchy. A common format
can preserve only a shared subset or carry vendor extensions.

## Direction for the Calculator

The Calculator should eventually have two deliberately separate capabilities.

### Native project save

A small, documented, versioned schema should preserve editing intent:

```text
project
├── format version
├── tempo, rate, and swing
├── track sound assignments
├── pattern bank
│   └── step intensity levels
└── chain
    ├── pattern references
    └── repetitions
```

The schema should be bounded, independent of transient UI selection where
possible, and protected by a length check and checksum when stored in flash or
removable media. The on-device encoding may be compact binary while a tooling
representation is documented as JSON. The stable contract is the meaning of
the fields, not necessarily the firmware's in-memory layout or disk bytes.

### Standard interchange

SMF export should preserve the resulting performance for other software. It
may flatten the chain, render swing into event positions, translate intensity
to velocity, and approximate AMY drums with General MIDI percussion notes.
Limited import could be considered later, but arbitrary MIDI cannot reliably
reconstruct the author's pattern boundaries and settings.

## Implication for the preset drummer showcase

The proposed electronic drummer is a complementary composition target:

- the Calculator lets the musician author patterns and chains;
- the drummer selects immutable curated grooves and tempo;
- both may produce timed one-shot drum triggers;
- only the Calculator currently needs a complete editable project.

The drummer should initially keep its groove representation local. Comparing
its authored cycles with the Calculator can reveal whether the useful shared
concept is a drum trigger, trigger lane, cyclic pattern, rhythmic position, or
pattern order entry. Serialization should follow those vocabulary findings,
not lead them.

## Sources

- [MIDI Association: Standard MIDI Files](https://midi.org/standard-midi-files)
- [MIDI Association: MIDI Clip File Specification](https://midi.org/midi-clip-file-specification-smf-midi-2.0)
- [MIDI Association: 2026 MIDI 2.0 file-interchange status](https://midi.org/the-state-of-midi-2-0-high-resolution-performance-and-the-rise-of-profiles-update-feb-2026)
- [DAWproject specification and schemas](https://github.com/bitwig/dawproject)
- [MusicXML introduction](https://www.musicxml.com/publications/makemusic-recordare/notation-and-analysis/introduction/)
- [Elektron Digitakt user manual](https://elektron.se/wp-content/uploads/2024/09/Digitakt_User_Manual_ENG_OS1.51_231108.pdf)
- [Roland MC-101 product and project overview](https://www.roland.com/us/products/mc-101/)
- [Roland MC-101 clip-package loading](https://support.roland.com/hc/en-us/articles/14099821439771-MC-101-How-to-Load-an-MCZ-Expansion)
- [Ableton Move reference manual](https://cdn-resources.ableton.com/resources/pdfs/move-manual/1/2024-10-04/move1-manual-en.pdf)
- [Polyend Tracker downloads and release history](https://polyend.com/downloads/tracker-downloads/)
- [Polyend discussion of Tracker project interchange](https://backstage.polyend.com/t/tracker-mini-files/10023)

## Result

No existing standard should become the Calculator's native project model.
Tracker modules provide the most relevant structural prior art, DAWproject
demonstrates a modern open interchange boundary, and SMF remains the best
widely compatible future performance export. A compact open native schema
should be designed only after the preset drummer supplies a second composition
against which to test the shared musical vocabulary.
