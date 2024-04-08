
// --- HELPER FUNCTIONS FOR DISPLAYS ---
#include "render_functions.h"

// display the Level 7 logo as a bitmap
void launchSSD1306() {

  // initalize the screen
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // setup the OLED scren
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    for(;;); // Don't proceed, loop forever
  }

  // clear the display
  display.clearDisplay();

  // display the Level 7 logo
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(500);

}

// display a test frame on the LCD screen
void launchST7735S() {
  
  // setup the LCD screen
  Config_Init();
  LCD_Init();
  LCD_Clear(BLACK);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
  Paint_SetRotate(180);

  // write the active zones
  Paint_DrawString_EN(1, 1, "REQUESTING POWER:", &Font8, BLACK, BLUE);
  Paint_DrawString_EN(1, 25, "RECEIVING POWER:", &Font8, BLACK, RED);
  delay(2000);

}

// display the active zones on the LCD
void displayActiveZones() {

  // populate power request table:
  String displayString1 = String(requestingPower[0]) + ", " + String(requestingPower[1]) + ", " + String(requestingPower[2]) + ", " + String(requestingPower[3]) + ", " + String(requestingPower[4]) + ", " + String(requestingPower[5]);
  Paint_DrawString_EN(1, 13, displayString1.c_str(), &Font8, BLACK, BLUE);
  
  String displayString2 = String(recievingPower[0]) + ", " + String(recievingPower[1]) + ", " + String(recievingPower[2]) + ", " + String(recievingPower[3]) + ", " + String(recievingPower[4]) + ", " + String(recievingPower[5]);
  Paint_DrawString_EN(1, 37, displayString2.c_str(), &Font8, BLACK, RED);

  String displayString3 = "Power Band 1 ON: " + String(powerNeeded1);
  Paint_DrawString_EN(1, 49, displayString3.c_str(), &Font8, BLACK, GREEN);

  String displayString4 = "Power Band 2 ON: " + String(powerNeeded2);
  Paint_DrawString_EN(1, 61, displayString4.c_str(), &Font8, BLACK, GREEN);

  Paint_DrawString_EN(1, 73, "     ", &Font8, BLACK, WHITE);

}


void clearDisplayTimerSelection() {

  Paint_DrawString_EN(1, 73, "                             ", &Font8, BLACK, WHITE);

}


void displayTimerSelection() {

  clearDisplayTimerSelection();
  String displayTimerSelectionMessage =  "Set Cook Time: " + String(cookTime) + " mins";
  Paint_DrawString_EN(1, 73, displayTimerSelectionMessage.c_str(), &Font8, BLACK, WHITE);

}


void updateDisplayRemainingCookTime(unsigned long remainingTimeMiliseconds) {

  // Convert the remaining ticks (miliseconds) to minutes 
  int remainingCookTime = (remainingTimeMiliseconds / 60)/1000;
  
  // Clear line before writing message
  clearDisplayTimerSelection();

  // Write the message
  String remainingCookTimeMessage = "Remaining Cook Time: " + String(remainingCookTime) + " mins";
  Paint_DrawString_EN(1, 73,  remainingCookTimeMessage.c_str(), &Font8, BLACK, WHITE);

}
