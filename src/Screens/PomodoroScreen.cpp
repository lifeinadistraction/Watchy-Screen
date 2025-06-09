#include "PomodoroScreen.h"
#include "Watchy.h"
#include "Fonts/OptimaLTStd7pt7b.h"
#include "Fonts/OptimaLTStd22pt7b.h"

PomodoroScreen::PomodoroScreen() {
    // Initialize timer state
    isRunning = false;
    remainingMinutes = POMODORO_DURATION;
    lastUpdateTime = 0;
}

void PomodoroScreen::show() {
    Watchy::display.fillScreen(GxEPD_BLACK);
    Watchy::display.setTextColor(GxEPD_WHITE);
    Watchy::display.setTextWrap(false);
    
    // Draw title
    Watchy::display.setFont(OptimaLTStd7pt7b);
    Watchy::display.setCursor(0, 20);
    Watchy::display.println("Pomodoro Timer");
    
    // Draw timer
    Watchy::display.setFont(OptimaLTStd22pt7b);
    char timeStr[6];
    sprintf(timeStr, "%02d:00", remainingMinutes);
    
    // Center the time display
    int16_t x1, y1;
    uint16_t w, h;
    Watchy::display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    int xPos = (200 - w) / 2;  // Center horizontally (200 is display width)
    int yPos = 100;  // Position vertically
    
    Watchy::display.setCursor(xPos, yPos);
    Watchy::display.println(timeStr);
    
    // Draw status
    Watchy::display.setFont(OptimaLTStd7pt7b);
    Watchy::display.setCursor(0, 150);
    Watchy::display.println(isRunning ? "Running" : "Paused");
    
    // Draw instructions
    Watchy::display.setCursor(0, 180);
    Watchy::display.println("Press MENU to start/pause");
    Watchy::display.println("Press BACK to reset");
    
    Watchy::display.display(true);
}

void PomodoroScreen::update() {
    if (isRunning) {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime >= 60000) {  // Update every minute
            if (remainingMinutes > 0) {
                remainingMinutes--;
                lastUpdateTime = currentTime;
                show();  // Update display
            } else {
                // Timer finished
                isRunning = false;
                // TODO: Add notification or sound
                show();
            }
        }
    }
}

void PomodoroScreen::menu() {
    isRunning = !isRunning;
    if (isRunning) {
        lastUpdateTime = millis();
    }
    show();
}

void PomodoroScreen::back() {
    // Reset timer
    remainingMinutes = POMODORO_DURATION;
    isRunning = false;
    show();
} 