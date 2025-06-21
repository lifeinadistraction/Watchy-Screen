#include "PomodoroScreen.h"
#include "Watchy.h"
#include "SevenSeg/DSEG7_Classic_Bold_53.h"
#include "SevenSeg/DSEG7_Classic_Bold_25.h"
#include "SevenSeg/icons7Seg.bold.h"

RTC_DATA_ATTR bool PomodoroScreen::isRunning = false;
RTC_DATA_ATTR int PomodoroScreen::remainingMinutes = 25;
RTC_DATA_ATTR unsigned long PomodoroScreen::lastUpdateTime = 0;

PomodoroScreen::PomodoroScreen() {
}

PomodoroScreen::~PomodoroScreen() {
}

void PomodoroScreen::show() {
    // Decrement timer if running and more than a minute has passed
    if (isRunning) {
        // Use RTC unixtime which is persistent across deep sleep
        tmElements_t tm;
        Watchy::RTC.read(tm);
        unsigned long now = makeTime(tm); 
        if (now - lastUpdateTime >= 60) { // 60 seconds
            int minutesPassed = (now - lastUpdateTime) / 60;
            remainingMinutes -= minutesPassed;
            if (remainingMinutes <= 0) {
                isRunning = false;
                remainingMinutes = POMODORO_DURATION;
            }
            // This handles cases where the refresh is not exactly every minute.
            lastUpdateTime += minutesPassed * 60;
        }
    }

    Watchy::display.fillScreen(GxEPD_WHITE);
    
    // Display timer with 7-segment font (Bold 53)
    Watchy::display.setFont(&DSEG7_Classic_Bold_53);
    Watchy::display.setTextColor(GxEPD_BLACK);
    
    // Calculate positions for the numbers
    int tens = remainingMinutes / 10;
    int ones = remainingMinutes % 10;
    
    // Display tens digit
    Watchy::display.setCursor(40, 130);
    Watchy::display.print(tens);
    
    // Display ones digit (moved closer)
    Watchy::display.setCursor(100, 130);
    Watchy::display.print(ones);
    
    // Display battery icon and fill segments
    float VBAT = Watchy::getBatteryVoltage();
    float batStat = Watchy::getBatteryState(VBAT);
    const int BATTERY_SEGMENT_COUNT = 3;
    const int BATTERY_SEGMENT_WIDTH = 7;
    const int BATTERY_SEGMENT_HEIGHT = 11;
    const int BATTERY_SEGMENT_SPACING = BATTERY_SEGMENT_WIDTH + 2;
    const int BATTERY_SEGMENTS_WIDTH = (BATTERY_SEGMENT_COUNT * BATTERY_SEGMENT_WIDTH);
    int batteryX = 154;
    int batteryY = 10;
    Watchy::display.drawBitmap(batteryX, batteryY, battery37x21, 37, 21, GxEPD_BLACK);
    Watchy::display.fillRect(batteryX+5, batteryY+5, 27, BATTERY_SEGMENT_HEIGHT, GxEPD_WHITE); // clear battery segments
    for( int8_t batterySegment = 0, fillWidth = (batStat * BATTERY_SEGMENTS_WIDTH) + 0.5f; 
        (batterySegment < BATTERY_SEGMENT_COUNT) && (fillWidth > 0); 
        batterySegment++, fillWidth -= BATTERY_SEGMENT_WIDTH){
        int8_t segmentWidth = ((fillWidth >= BATTERY_SEGMENT_WIDTH) ? BATTERY_SEGMENT_WIDTH : fillWidth );
        if( segmentWidth < 1 )
            break;
        Watchy::display.fillRect(batteryX+5 + (batterySegment * BATTERY_SEGMENT_SPACING), batteryY+5, segmentWidth, BATTERY_SEGMENT_HEIGHT, GxEPD_BLACK);
    }
    
    // Display airplane_mode_on21x21 icon if running, otherwise nothing
    if(isRunning) {
        Watchy::display.drawBitmap(100, 160, airplane_mode_on21x21, 21, 21, GxEPD_BLACK);
    }
}

void PomodoroScreen::menu() {
    isRunning = !isRunning;
    if (isRunning) {
        if (remainingMinutes <= 0) {
            remainingMinutes = POMODORO_DURATION;
        }
        // Use RTC unixtime which is persistent across deep sleep
        tmElements_t tm;
        Watchy::RTC.read(tm);
        lastUpdateTime = makeTime(tm);
    }
    show();
    Watchy::showWatchFace(true);
}

void PomodoroScreen::back() {
    remainingMinutes = 25;
    isRunning = false;
    show();
    Watchy::showWatchFace(true);
}

void PomodoroScreen::up() {
    // Not used in Pomodoro screen
}

void PomodoroScreen::down() {
    // Not used in Pomodoro screen
}

void PomodoroScreen::update() {
    // Not needed with new logic
}

void PomodoroScreen::updateTimer(const esp_sleep_wakeup_cause_t wakeup_reason) {
    // Not needed with new logic
} 