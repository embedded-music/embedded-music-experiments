#include <Arduino.h>
#include <M5Unified.h>
#include <esp_timer.h>

#include "AmyM5TriggerOutput.h"
#include "DrummerPresets.h"
#include "TriggerPatternPlayer.h"

namespace {
constexpr uint16_t INITIAL_BPM = 120;
constexpr uint16_t MINIMUM_BPM = 40;
constexpr uint16_t MAXIMUM_BPM = 240;
constexpr uint16_t BPM_INCREMENT = 5;

AmyM5TriggerOutput drumOutput;
DrummerPreset preset;
class PresetPatternSource final : public TriggerPatternSource {
 public:
  const TriggerPattern& currentPattern() const override {
    return preset.pattern;
  }
};
PresetPatternSource patternSource;
TriggerPatternPlayer patternPlayer(patternSource, drumOutput);
DrummerStyle style = DrummerStyle::Rock;
uint16_t bpm = INITIAL_BPM;

uint64_t stepIntervalUs() {
  return 60000000ULL / (static_cast<uint64_t>(bpm) * preset.stepsPerBeat);
}

void drawScreen() {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setRotation(1);
  M5.Display.setTextColor(TFT_CYAN, TFT_BLACK);
  M5.Display.setTextSize(2);
  M5.Display.setCursor(12, 14);
  M5.Display.println("ELECTRONIC DRUMMER");
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setTextSize(4);
  M5.Display.setCursor(12, 62);
  M5.Display.println(preset.name);
  M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Display.setTextSize(3);
  M5.Display.setCursor(12, 118);
  M5.Display.printf("%u BPM", bpm);
  M5.Display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  M5.Display.setTextSize(2);
  M5.Display.setCursor(12, 186);
  M5.Display.print("A STYLE   B -   C +");
}

void configureTriggerVoices() {
  for (uint8_t lane = 0; lane < TriggerPattern::LANE_COUNT; lane++) {
    drumOutput.configureLane(
        lane,
        AmyTriggerVoice{
            preset.midiNotes[lane], {}, preset.midiNotes[lane] != 0});
  }
}

void selectNextStyle(uint64_t nowUs) {
  style = nextDrummerStyle(style);
  loadDrummerPreset(style, preset);
  configureTriggerVoices();
  if (!patternPlayer.restart(
          nowUs, StepIntervals::constant(stepIntervalUs()),
          TriggerStart::EmitImmediately)) {
    Serial.println("clock: style_reschedule_failed");
  }
  drawScreen();
  Serial.printf("style=%s steps=%u steps_per_beat=%u\n", preset.name,
                preset.pattern.length(), preset.stepsPerBeat);
}

void changeTempo(int16_t delta, uint64_t nowUs) {
  const int16_t requested = static_cast<int16_t>(bpm) + delta;
  const uint16_t next = requested < MINIMUM_BPM
                            ? MINIMUM_BPM
                            : requested > MAXIMUM_BPM ? MAXIMUM_BPM : requested;
  if (next == bpm) return;
  bpm = next;
  if (!patternPlayer.changeTiming(
          nowUs, StepIntervals::constant(stepIntervalUs()),
          TriggerTimingChange::PreservePhase)) {
    Serial.println("clock: tempo_reschedule_failed");
  }
  drawScreen();
  Serial.printf("tempo_bpm=%u\n", bpm);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);
  auto config = M5.config();
  config.internal_spk = true;
  config.internal_mic = false;
  M5.begin(config);

  loadDrummerPreset(style, preset);
  configureTriggerVoices();
  drawScreen();
  drumOutput.begin();
  const uint64_t nowUs = static_cast<uint64_t>(esp_timer_get_time());
  if (!patternPlayer.begin(nowUs,
                           StepIntervals::constant(stepIntervalUs()),
                           TriggerStart::EmitImmediately)) {
    Serial.println("clock: begin_failed");
  }
  Serial.printf("electronic_drummer: style=%s tempo_bpm=%u\n", preset.name,
                bpm);
}

void loop() {
  M5.update();
  const uint64_t nowUs = static_cast<uint64_t>(esp_timer_get_time());

  if (M5.BtnA.wasPressed()) selectNextStyle(nowUs);
  if (M5.BtnB.wasPressed()) changeTempo(-BPM_INCREMENT, nowUs);
  if (M5.BtnC.wasPressed()) changeTempo(BPM_INCREMENT, nowUs);

  const TriggerPatternPlayerUpdate playback = patternPlayer.update(nowUs);
  if (playback.elapsedSteps > 1) {
    Serial.printf("playback: skipped_steps=%lu\n",
                  static_cast<unsigned long>(playback.elapsedSteps));
  }

  drumOutput.update();
  delay(1);
}
