#include "PomodoroScreen.h"
#include "Watchy.h"
#include "SevenSeg/DSEG7_Classic_Bold_53.h"
#include "SevenSeg/DSEG7_Classic_Bold_25.h"
#include "SevenSeg/icons7Seg.bold.h"
#include "Fonts/FreeMonoBold9pt7b.h"

RTC_DATA_ATTR bool PomodoroScreen::isRunning = false;
RTC_DATA_ATTR unsigned long PomodoroScreen::startTime = 0;

PomodoroScreen::PomodoroScreen() {
}

PomodoroScreen::~PomodoroScreen() {
}

void PomodoroScreen::show() {
    Watchy::RTC.setRefresh(RTC_REFRESH_MIN); // Ensure minute wakeup while Pomodoro is active
    int remainingMinutes = 25;
    unsigned long now = 0;
    unsigned long elapsed_seconds = 0;
    
    if (isRunning) {
        tmElements_t tm;
        Watchy::RTC.read(tm);
        now = makeTime(tm);
        elapsed_seconds = now - startTime;
        int minutesPassed = elapsed_seconds / 60;
        remainingMinutes = 25 - minutesPassed;
        if (remainingMinutes <= 0) {
            isRunning = false;
            remainingMinutes = 25;
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
        Watchy::display.drawBitmap(160, 80, airplane_mode_on21x21, 21, 21, GxEPD_BLACK);
    }

    // DEBUGGING OUTPUT
    Watchy::display.setFont(&FreeMonoBold9pt7b);
    Watchy::display.setCursor(0, 180);
    Watchy::display.print("elapsed: ");
    Watchy::display.print(elapsed_seconds);
    Watchy::display.setCursor(0, 195);
    Watchy::display.print("running: ");
    Watchy::display.print(isRunning ? "T" : "F");
}

void PomodoroScreen::menu() {
    isRunning = !isRunning;
    if (isRunning) {
        tmElements_t tm;
        Watchy::RTC.read(tm);
        startTime = makeTime(tm);
    }
    Watchy::showWatchFace(true);
}

void PomodoroScreen::back() {
    isRunning = false;
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