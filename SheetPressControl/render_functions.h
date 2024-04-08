#ifndef _RENDER_FUNCTIONS_H_
#define _RENDER_FUNCTIONS_H_

#include "globals.h"
#include "level_7_logo.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"

void launchSSD1306();
void launchST7735S();
void displayActiveZones();
void displayTimerSelection();
void clearDisplayTimerSelection();
void displayCookCountdown(); 
void updateDisplayRemainingCookTime(unsigned long remainingTimeMiliseconds);

#endif 