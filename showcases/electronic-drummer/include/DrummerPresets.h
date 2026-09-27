#pragma once

#include <stdint.h>

#include "TriggerPattern.h"

enum class DrummerStyle : uint8_t {
  Rock,
  Jazz,
  Bossa,
  EuroPop,
  Metronome,
  Off,
};

constexpr uint8_t DRUMMER_STYLE_COUNT = 6;

struct DrummerPreset {
  const char* name = "";
  uint8_t midiNotes[TriggerPattern::LANE_COUNT]{};
  uint8_t stepsPerBeat = 1;
  TriggerPattern pattern;
};

DrummerStyle nextDrummerStyle(DrummerStyle style);
void loadDrummerPreset(DrummerStyle style, DrummerPreset& preset);
