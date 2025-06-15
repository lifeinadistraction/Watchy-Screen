#pragma once

#include "Screen.h"
#include "Watchy.h"
#include "Fonts/OptimaLTStd12pt7b.h"
#include "Fonts/OptimaLTStd22pt7b.h"
#include "Events.h"

class PomodoroScreen : public Screen {
public:
    PomodoroScreen();
    void show() override;
    void menu() override;
    void back() override;
    void up() override;
    void down() override;
    void update();
    static void updateTimer(const esp_sleep_wakeup_cause_t wakeup_reason);
    static PomodoroScreen* instance;

private:
    static const int POMODORO_DURATION = 25;  // 25 minutes
    RTC_DATA_ATTR static bool isRunning;
    RTC_DATA_ATTR static int remainingMinutes;
    RTC_DATA_ATTR static unsigned long lastUpdateTime;
    Watchy_Event::BackgroundTask timerTask;
}; 