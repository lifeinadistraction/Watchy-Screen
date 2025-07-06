#include "PomodoroScreen.h"
#include "Watchy.h"

PomodoroScreen pomodoroScreen;

void setup() {
  Serial.begin(115200);

  if (Watchy::screen == nullptr) {
    Watchy::screen = &pomodoroScreen;
  }
  Watchy::init();
}

void loop() {
  // Should never be called; Watchy deep sleeps after init.
}