#include <Arduino.h>
#include <M5Unified.h>
#include <esp_timer.h>

#include "AmyAudioActivityGate.h"
#include "AmyM5SpeakerBridge.h"
#include "AmySynthSlot.h"
#include "DeadlineClock.h"
#include "DrummerPresets.h"
#include "TriggerEventSink.h"
#include "TriggerPatternPlayer.h"

namespace {
constexpr uint8_t AMY_SYNTH_ID = 1;
constexpr uint8_t AMY_DRUM_VOICES = 1;
constexpr uint16_t AMY_GM_DRUM_PATCH = 258;
constexpr uint16_t INITIAL_BPM = 120;
constexpr uint16_t MINIMUM_BPM = 40;
constexpr uint16_t MAXIMUM_BPM = 240;
constexpr uint16_t BPM_INCREMENT = 5;
constexpr uint32_t DRUM_TAIL_MS = 1500;

float velocityForStepLevel(StepLevel level) {
  switch (level) {
    case StepLevel::Weak: return 0.45f;
    case StepLevel::Normal: return 0.70f;
    case StepLevel::Strong: return 1.0f;
    case StepLevel::Off: return 0.0f;
  }
  return 0.0f;
}

class AmyPresetSink final : public TriggerEventSink {
 public:
  AmyPresetSink(AmyAudioActivityGate& gate, AmySynthSlot& slot,
                const DrummerPreset& preset)
      : gate_(gate), slot_(slot), preset_(preset) {}

  void trigger(const TriggerEvent& event) override {
    if (!awake_) {
      gate_.wake(DRUM_TAIL_MS);
      awake_ = true;
    }
    slot_.noteOn(preset_.midiNotes[event.lane],
                 velocityForStepLevel(event.level));
  }

 private:
  AmyAudioActivityGate& gate_;
  AmySynthSlot& slot_;
  const DrummerPreset& preset_;
  bool awake_ = false;
};

AmyM5SpeakerBridge amyBridge;
AmyAudioActivityGate audioGate(amyBridge);
AmySynthSlot drumSlot;
DeadlineClock stepClock;
DrummerPreset preset;
DrummerStyle style = DrummerStyle::Rock;
uint16_t bpm = INITIAL_BPM;
uint8_t position = 0;

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

void triggerPosition() {
  AmyPresetSink sink(audioGate, drumSlot, preset);
  TriggerPatternPlayer::emitStep(preset.pattern, position, sink);
}

void selectNextStyle(uint64_t nowUs) {
  style = nextDrummerStyle(style);
  loadDrummerPreset(style, preset);
  position = 0;
  if (!stepClock.reschedule(nowUs, stepIntervalUs(),
                            IntervalChangePolicy::ResetFromNow)) {
    Serial.println("clock: style_reschedule_failed");
  }
  drawScreen();
  triggerPosition();
  Serial.printf("style=%s positions=%u steps_per_beat=%u\n", preset.name,
                preset.pattern.length(), preset.stepsPerBeat);
}

void changeTempo(int16_t delta, uint64_t nowUs) {
  const int16_t requested = static_cast<int16_t>(bpm) + delta;
  const uint16_t next = requested < MINIMUM_BPM
                            ? MINIMUM_BPM
                            : requested > MAXIMUM_BPM ? MAXIMUM_BPM : requested;
  if (next == bpm) return;
  bpm = next;
  if (!stepClock.reschedule(nowUs, stepIntervalUs(),
                            IntervalChangePolicy::PreservePhase)) {
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
  drawScreen();
  amyBridge.begin();
  M5.Speaker.setVolume(128);
  drumSlot.begin(AMY_SYNTH_ID, AMY_DRUM_VOICES, AMY_GM_DRUM_PATCH);
  const uint64_t nowUs = static_cast<uint64_t>(esp_timer_get_time());
  if (!stepClock.begin(nowUs, stepIntervalUs())) {
    Serial.println("clock: begin_failed");
  }
  triggerPosition();
  Serial.printf("electronic_drummer: style=%s tempo_bpm=%u\n", preset.name,
                bpm);
}

void loop() {
  M5.update();
  const uint64_t nowUs = static_cast<uint64_t>(esp_timer_get_time());

  if (M5.BtnA.wasPressed()) selectNextStyle(nowUs);
  if (M5.BtnB.wasPressed()) changeTempo(-BPM_INCREMENT, nowUs);
  if (M5.BtnC.wasPressed()) changeTempo(BPM_INCREMENT, nowUs);

  const ClockAdvance advance = stepClock.poll(nowUs);
  if (advance.elapsed_intervals > 0) {
    position = static_cast<uint8_t>(
        (position + advance.elapsed_intervals) % preset.pattern.length());
    if (advance.elapsed_intervals == 1) triggerPosition();
    else Serial.printf("transport: skipped_positions=%lu\n",
                       static_cast<unsigned long>(advance.elapsed_intervals));
  }

  audioGate.update(false);
  delay(1);
}
