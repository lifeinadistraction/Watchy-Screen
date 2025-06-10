#include "PomodoroScreen.h"
#include "Watchy.h"
#include "Fonts/OptimaLTStd7pt7b.h"
#include "Fonts/OptimaLTStd22pt7b.h"

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
    
    // Display timer with large font
    Watchy::display.setFont(OptimaLTStd22pt7b);
    Watchy::display.setTextColor(GxEPD_BLACK);
    char timeStr[6];
    sprintf(timeStr, "%02d:00", remainingMinutes);
    Watchy::display.setCursor(50, 80);
    Watchy::display.print(timeStr);
    
    // Display status with smaller font
    Watchy::display.setFont(OptimaLTStd12pt7b);
    Watchy::display.setCursor(50, 120);
    Watchy::display.print(isRunning ? "Running" : "Paused");
    
    // Display instructions
    Watchy::display.setCursor(20, 160);
    Watchy::display.print("MENU: Start/Stop");
    Watchy::display.setCursor(20, 180);
    Watchy::display.print("BACK: Reset");
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

void PomodoroScreen::update() {
    // This method is no longer needed as updates are handled by the BackgroundTask
}

void PomodoroScreen::updateTimer(const esp_sleep_wakeup_cause_t wakeup_reason) {
    // This method is no longer needed as updates are handled by the BackgroundTask
} 