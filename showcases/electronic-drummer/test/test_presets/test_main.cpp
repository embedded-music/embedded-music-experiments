#include <unity.h>

#include "DrummerPresets.h"

void test_presets_use_distinct_cycle_grids() {
  DrummerPreset preset;

  loadDrummerPreset(DrummerStyle::Rock, preset);
  TEST_ASSERT_EQUAL_UINT8(16, preset.pattern.length());
  TEST_ASSERT_EQUAL_UINT8(4, preset.stepsPerBeat);

  loadDrummerPreset(DrummerStyle::Jazz, preset);
  TEST_ASSERT_EQUAL_UINT8(12, preset.pattern.length());
  TEST_ASSERT_EQUAL_UINT8(3, preset.stepsPerBeat);

  loadDrummerPreset(DrummerStyle::EuroPop, preset);
  TEST_ASSERT_EQUAL_UINT8(16, preset.pattern.length());

  loadDrummerPreset(DrummerStyle::Metronome, preset);
  TEST_ASSERT_EQUAL_UINT8(4, preset.pattern.length());
  TEST_ASSERT_EQUAL_UINT8(1, preset.stepsPerBeat);
}

void test_style_cycle_returns_to_rock() {
  DrummerStyle style = DrummerStyle::Rock;
  for (uint8_t index = 0; index < DRUMMER_STYLE_COUNT; index++) {
    style = nextDrummerStyle(style);
  }
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(DrummerStyle::Rock),
                          static_cast<uint8_t>(style));
}

void test_metronome_accents_first_beat() {
  DrummerPreset preset;
  loadDrummerPreset(DrummerStyle::Metronome, preset);

  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Strong),
                          static_cast<uint8_t>(preset.pattern.stepLevel(0, 0)));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(StepLevel::Normal),
                          static_cast<uint8_t>(preset.pattern.stepLevel(1, 1)));
}

void test_off_style_has_one_repeating_silent_step() {
  DrummerPreset preset;
  loadDrummerPreset(DrummerStyle::Off, preset);

  TEST_ASSERT_EQUAL_UINT8(1, preset.pattern.length());
  TEST_ASSERT_EQUAL_UINT8(1, preset.stepsPerBeat);
  TEST_ASSERT_TRUE(preset.pattern.empty());
}

void test_bossa_uses_two_bar_source_pattern() {
  DrummerPreset preset;
  loadDrummerPreset(DrummerStyle::Bossa, preset);

  TEST_ASSERT_EQUAL_UINT8(32, preset.pattern.length());
  TEST_ASSERT_EQUAL_UINT8(4, preset.stepsPerBeat);
  TEST_ASSERT_EQUAL_UINT8(35, preset.midiNotes[0]);
  TEST_ASSERT_EQUAL_UINT8(79, preset.midiNotes[1]);
  TEST_ASSERT_EQUAL_UINT8(70, preset.midiNotes[2]);
  TEST_ASSERT_EQUAL_UINT8(42, preset.midiNotes[3]);
  TEST_ASSERT_TRUE(preset.pattern.stepActive(0, 28));
  TEST_ASSERT_TRUE(preset.pattern.stepActive(1, 30));
}

void test_jazz_uses_maracas_without_a_duplicate_fourth_lane() {
  DrummerPreset preset;
  loadDrummerPreset(DrummerStyle::Jazz, preset);

  TEST_ASSERT_EQUAL_UINT8(70, preset.midiNotes[2]);
  for (uint8_t step = 0; step < preset.pattern.length(); step++) {
    TEST_ASSERT_EQUAL_UINT8(
        static_cast<uint8_t>(StepLevel::Off),
        static_cast<uint8_t>(preset.pattern.stepLevel(3, step)));
  }
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_presets_use_distinct_cycle_grids);
  RUN_TEST(test_style_cycle_returns_to_rock);
  RUN_TEST(test_metronome_accents_first_beat);
  RUN_TEST(test_off_style_has_one_repeating_silent_step);
  RUN_TEST(test_bossa_uses_two_bar_source_pattern);
  RUN_TEST(test_jazz_uses_maracas_without_a_duplicate_fourth_lane);
  return UNITY_END();
}
