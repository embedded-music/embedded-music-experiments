#include "DrummerPresets.h"

#include <stddef.h>

#include "TriggerPatternCell.h"
#include "TriggerPatternLoader.h"

namespace {

template <size_t CellCount>
void loadPreset(DrummerPreset& preset, const char* name, uint8_t length,
                uint8_t stepsPerBeat,
                const uint8_t (&midiNotes)[TriggerPattern::LANE_COUNT],
                const TriggerPatternCell (&cells)[CellCount]) {
  preset.name = name;
  preset.stepsPerBeat = stepsPerBeat;
  TriggerPatternLoader::load(preset.pattern, length, cells);
  for (uint8_t lane = 0; lane < TriggerPattern::LANE_COUNT; lane++) {
    preset.midiNotes[lane] = midiNotes[lane];
  }
}

void loadRock(DrummerPreset& preset) {
  constexpr uint8_t notes[] = {36, 38, 42, 46};
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Strong}, {0, 8, StepLevel::Normal},
      {1, 4, StepLevel::Normal}, {1, 12, StepLevel::Strong},
      {2, 0, StepLevel::Normal}, {2, 2, StepLevel::Weak},
      {2, 4, StepLevel::Normal}, {2, 6, StepLevel::Weak},
      {2, 8, StepLevel::Normal}, {2, 10, StepLevel::Weak},
      {2, 12, StepLevel::Normal}, {3, 14, StepLevel::Normal},
  };
  loadPreset(preset, "ROCK", 16, 4, notes, cells);
}

void loadJazz(DrummerPreset& preset) {
  constexpr uint8_t notes[] = {36, 38, 70, 0};
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Weak}, {0, 6, StepLevel::Weak},
      {1, 3, StepLevel::Weak}, {1, 9, StepLevel::Weak},
      {2, 0, StepLevel::Strong}, {2, 2, StepLevel::Weak},
      {2, 3, StepLevel::Normal}, {2, 5, StepLevel::Weak},
      {2, 6, StepLevel::Normal}, {2, 8, StepLevel::Weak},
      {2, 9, StepLevel::Normal}, {2, 11, StepLevel::Weak},
  };
  loadPreset(preset, "JAZZ", 12, 3, notes, cells);
}

void loadBossa(DrummerPreset& preset) {
  constexpr uint8_t notes[] = {35, 79, 70, 42};
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Normal}, {0, 6, StepLevel::Normal},
      {0, 14, StepLevel::Normal}, {0, 16, StepLevel::Normal},
      {0, 22, StepLevel::Normal}, {0, 28, StepLevel::Weak},
      {1, 0, StepLevel::Normal}, {1, 10, StepLevel::Normal},
      {1, 16, StepLevel::Normal}, {1, 22, StepLevel::Normal},
      {1, 30, StepLevel::Normal},
      {2, 0, StepLevel::Normal}, {2, 2, StepLevel::Weak},
      {2, 4, StepLevel::Normal}, {2, 6, StepLevel::Weak},
      {2, 8, StepLevel::Normal}, {2, 10, StepLevel::Weak},
      {2, 12, StepLevel::Normal}, {2, 14, StepLevel::Weak},
      {2, 16, StepLevel::Normal}, {2, 18, StepLevel::Weak},
      {2, 20, StepLevel::Normal}, {2, 22, StepLevel::Weak},
      {2, 24, StepLevel::Normal}, {2, 26, StepLevel::Weak},
      {2, 28, StepLevel::Normal}, {2, 30, StepLevel::Weak},
      {3, 0, StepLevel::Weak}, {3, 4, StepLevel::Weak},
      {3, 8, StepLevel::Weak}, {3, 12, StepLevel::Weak},
      {3, 16, StepLevel::Weak}, {3, 20, StepLevel::Weak},
      {3, 24, StepLevel::Weak}, {3, 28, StepLevel::Weak},
  };
  loadPreset(preset, "BOSSA", 32, 4, notes, cells);
}

void loadEuroPop(DrummerPreset& preset) {
  constexpr uint8_t notes[] = {36, 39, 42, 46};
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Strong}, {0, 4, StepLevel::Strong},
      {0, 8, StepLevel::Strong}, {0, 12, StepLevel::Strong},
      {1, 4, StepLevel::Normal}, {1, 12, StepLevel::Normal},
      {2, 2, StepLevel::Weak}, {2, 6, StepLevel::Weak},
      {2, 10, StepLevel::Weak}, {2, 14, StepLevel::Weak},
      {3, 7, StepLevel::Normal}, {3, 15, StepLevel::Normal},
  };
  loadPreset(preset, "EURO POP", 16, 4, notes, cells);
}

void loadMetronome(DrummerPreset& preset) {
  constexpr uint8_t notes[] = {76, 77, 0, 0};
  constexpr TriggerPatternCell cells[] = {
      {0, 0, StepLevel::Strong},
      {1, 1, StepLevel::Normal},
      {1, 2, StepLevel::Normal},
      {1, 3, StepLevel::Normal},
  };
  loadPreset(preset, "METRONOME", 4, 1, notes, cells);
}

void loadOff(DrummerPreset& preset) {
  preset.name = "OFF";
  preset.stepsPerBeat = 1;
  preset.pattern.clear();
  preset.pattern.setLength(1);
  for (uint8_t lane = 0; lane < TriggerPattern::LANE_COUNT; lane++) {
    preset.midiNotes[lane] = 0;
  }
}

}  // namespace

DrummerStyle nextDrummerStyle(DrummerStyle style) {
  const uint8_t next = (static_cast<uint8_t>(style) + 1) % DRUMMER_STYLE_COUNT;
  return static_cast<DrummerStyle>(next);
}

void loadDrummerPreset(DrummerStyle style, DrummerPreset& preset) {
  switch (style) {
    case DrummerStyle::Rock:
      loadRock(preset);
      return;
    case DrummerStyle::Jazz:
      loadJazz(preset);
      return;
    case DrummerStyle::Bossa:
      loadBossa(preset);
      return;
    case DrummerStyle::EuroPop:
      loadEuroPop(preset);
      return;
    case DrummerStyle::Metronome:
      loadMetronome(preset);
      return;
    case DrummerStyle::Off:
      loadOff(preset);
      return;
  }
}
