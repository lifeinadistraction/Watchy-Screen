#include "PomodoroScreen.h"
#include "Watchy.h"
#include "Bahn-for-Watchy/DIN_1451_Engschrift_Regular64pt7b.h"
#include "SevenSeg/DSEG7_Classic_Bold_53.h"
#include "SevenSeg/DSEG7_Classic_Bold_25.h"
#include "SevenSeg/icons7Seg.bold.h"
#include "Fonts/FreeMonoBold9pt7b.h"

extern void buzz_gpio();

RTC_DATA_ATTR bool PomodoroScreen::isRunning = false;
RTC_DATA_ATTR unsigned long PomodoroScreen::startTime = 0;
RTC_DATA_ATTR int PomodoroScreen::refreshCount = 0;

PomodoroScreen::PomodoroScreen() {
}

PomodoroScreen::~PomodoroScreen() {
}

void PomodoroScreen::show() {
    if (isRunning) {
        Watchy::RTC.setRefresh(RTC_REFRESH_MIN); // Only set minute wakeup when running
    } else {
        Watchy::RTC.setRefresh(RTC_REFRESH_NONE); // Disable periodic wakeup when paused
    }
    refreshCount++;
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
            buzz_gpio();
        }
    }

    Watchy::display.fillScreen(GxEPD_WHITE);
    
    // Display timer with DIN 1451 Engschrift font
    Watchy::display.setFont(&DIN_1451_Engschrift_Regular64pt7b);
    Watchy::display.setTextColor(GxEPD_BLACK);
    
    // Initialize variables
    int16_t  x1, y1, lasty;
    uint16_t w, h;
    
    // Display remaining minutes
    String textstring;
    if (remainingMinutes < 10) {
        textstring = "0";
      } else {
        textstring = "";
      }
    textstring += remainingMinutes;
    Watchy::display.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
    Watchy::display.setCursor(100-w, 100-5);
    Watchy::display.print(textstring);
    
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

    // DEBUGGING OUTPUT
    Watchy::display.setFont(&FreeMonoBold9pt7b);
    Watchy::display.setCursor(0, 165);
    Watchy::display.print("refresh: ");
    Watchy::display.print(refreshCount);
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
        buzz_gpio();
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