#include "PomodoroScreen.h"
#include "Watchy.h"
#include "Fonts/OptimaLTStd7pt7b.h"
#include "Fonts/OptimaLTStd12pt7b.h"
#include "Fonts/OptimaLTStd22pt7b.h"
#include "SevenSeg/DSEG7_Classic_Bold_53.h"
#include "SevenSeg/DSEG7_Classic_Bold_25.h"

PomodoroScreen* PomodoroScreen::instance = nullptr;
bool PomodoroScreen::isRunning = false;
int PomodoroScreen::remainingMinutes = 25;
unsigned long PomodoroScreen::lastUpdateTime = 0;

PomodoroScreen::PomodoroScreen() : 
    timerTask("pomodoroTimer", [this]() {
        while (isRunning) {
            vTaskDelay(60000 / portTICK_PERIOD_MS); // Wait for 1 minute
            if (isRunning) {
                remainingMinutes--;
                if (remainingMinutes <= 0) {
                    isRunning = false;
                    remainingMinutes = POMODORO_DURATION;
                }
                show();
                Watchy::showWatchFace(true);
            }
        }
    })
{
    instance = this;
}

void PomodoroScreen::show() {
    Watchy::display.fillScreen(GxEPD_WHITE);
    
    // Display timer with 7-segment font (Bold 53)
    Watchy::display.setFont(&DSEG7_Classic_Bold_53);
    Watchy::display.setTextColor(GxEPD_BLACK);
    
    // Calculate positions for the numbers
    int tens = remainingMinutes / 10;
    int ones = remainingMinutes % 10;
    
    // Display tens digit
    Watchy::display.setCursor(40, 90);
    Watchy::display.print(tens);
    
    // Display ones digit (moved closer)
    Watchy::display.setCursor(100, 90);
    Watchy::display.print(ones);
    
    // Display status with DSEG7_Classic_Bold_25 font
    Watchy::display.setFont(&DSEG7_Classic_Bold_25);
    Watchy::display.setCursor(50, 160);
    Watchy::display.print(isRunning ? "Running" : "Paused");
}

void PomodoroScreen::menu() {
    isRunning = !isRunning;
    if (isRunning) {
        lastUpdateTime = millis();
        timerTask.begin();
    } else {
        timerTask.kill();
    }
    show();
    Watchy::showWatchFace(true);
}

void PomodoroScreen::back() {
    remainingMinutes = 25;
    isRunning = false;
    show();
}

void PomodoroScreen::up() {
    // Not used in Pomodoro screen
}

void PomodoroScreen::down() {
    // Not used in Pomodoro screen
}

void PomodoroScreen::update() {
    // This method is no longer needed as updates are handled by the BackgroundTask
}

void PomodoroScreen::updateTimer(const esp_sleep_wakeup_cause_t wakeup_reason) {
    // This method is no longer needed as updates are handled by the BackgroundTask
} 