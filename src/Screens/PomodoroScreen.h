#pragma once

#include "Screen.h"
#include "Events.h"

class PomodoroScreen : public Screen {
public:
    PomodoroScreen();
    ~PomodoroScreen();
    void show() override;
    void menu() override;
    void back() override;
    void up() override;
    void down() override;
    void update();
    void updateTimer(const esp_sleep_wakeup_cause_t wakeup_reason);

private:
    static PomodoroScreen* instance;
    RTC_DATA_ATTR static bool isRunning;
    RTC_DATA_ATTR static int remainingMinutes;
    RTC_DATA_ATTR static unsigned long lastUpdateTime;
    static const int POMODORO_DURATION = 25;
    
    void checkTimer();
}; 