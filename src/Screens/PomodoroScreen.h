#pragma once

#include "Screen.h"

class PomodoroScreen : public Screen {
public:
    PomodoroScreen();
    void show() override;
    void update();
    void menu() override;
    void back() override;

private:
    static const int POMODORO_DURATION = 25;  // 25 minutes
    bool isRunning;
    int remainingMinutes;
    unsigned long lastUpdateTime;
}; 