#pragma once

#include "Screen.h"
#include "PomodoroScreen.h"

class ScreenManager {
public:
    static Screen* getPomodoroScreen() {
        static PomodoroScreen pomodoroScreen;
        return &pomodoroScreen;
    }
}; 