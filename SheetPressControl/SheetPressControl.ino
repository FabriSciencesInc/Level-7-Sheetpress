
#include <SPI.h>
#include <Servo.h>
#include <arduino-timer.h>
#include <Wire.h>
#include "level_7_logo.h"
#include "globals.h"
#include "render_functions.h"
#include "zone_control.h"
#include "rotary_encoder.h"


// --- INITALIZE GLOBAL PARAMETERS ---

const int PID_0 = 38;     // D4
const int PID_1 = 39;     // D5
const int PID_2 = 40;     // D6
const int PID_3 = 44;     // D7
const int PID_4 = 45;     // D8
const int PID_5 = 46;     // D9
const int Zone_0 = 41;    // D10
const int Zone_1 = 42;    // D11
const int Zone_2 = 43;    // D12
const int Zone_3 = 47;    // D13
const int Zone_4 = 48;    // D14
const int Zone_5 = 49;    // D15

const int coolingSolenoid = 15; 
const int heatingSolenoid = 16; 

const int coolingServo = 5; 
const int heatingServo = 6;

const int CLK = 2; 
const int DT = 3;
const int SW = 4; 

int counter = 0; 
int currentStateCLK; 
int lastStateCLK;

int defaultCookTime = 180;
int cookTimeIncriment = 15; 
int cookTime; 

// Array to hold PID power request information
int requestingPower[6] = {0,0,0,0,0,0};

// Array to know which zones are recieving power
int recievingPower[6] = {0,0,0,0,0,0};

// Array to store which zones are active
int activeZones[2] = {0,0};

// Keep track of if the two possible power zones are actually on
bool powerNeeded1 = false;
bool powerNeeded2 = false;

// Timer for executing sheetpress zone refresh (units in ms)
auto timer = timer_create_default();
const unsigned long zoneUpdatePeriod = 30000;


// --- SET GLOBAL STATES ---

CookState cookState = OFF;


// --- HEATING ELEMENT MANAGEMENT ---

bool sheetpressHeatingPipeline(void *) {

  if (cookState == true) {

    // Populate the power request vector for input to power assignment algorithum
    fillPowerRequestVector();

    // Determine which zones should recieve power
    findNextActiveZones();

    // Activate the SSRs in each respective zone
    switchSSRs();

    // Display which zones are currently active
    displayActiveZones();

  }

  // Return true to continue the execution of the timer
  return true; 

}


// --- SET LOCAL PARAMETERS ---

unsigned long cookEndTimeInMiliseconds; 


// --- SETUP ---

void setup() {

  // initialize pins for thermal control
  pinMode(PID_0, INPUT);
  pinMode(PID_1, INPUT);
  pinMode(PID_2, INPUT);
  pinMode(PID_3, INPUT);
  pinMode(PID_4, INPUT);
  pinMode(PID_5, INPUT);
  pinMode(Zone_0, OUTPUT);
  pinMode(Zone_1, OUTPUT);
  pinMode(Zone_2, OUTPUT);
  pinMode(Zone_3, OUTPUT);
  pinMode(Zone_4, OUTPUT);
  pinMode(Zone_5, OUTPUT);

  // initalize pins for air pressure
  pinMode(coolingSolenoid, OUTPUT);
  pinMode(heatingSolenoid, OUTPUT); 

  // initalize pins for rotery encoder
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT);

  // turn all SSRs off
  digitalWrite(Zone_0, LOW);
  digitalWrite(Zone_1, LOW);
  digitalWrite(Zone_2, LOW);
  digitalWrite(Zone_3, LOW);
  digitalWrite(Zone_4, LOW);
  digitalWrite(Zone_5, LOW);

  // ensure the solenoid valves are closed
  digitalWrite(coolingSolenoid, LOW);
  digitalWrite(heatingSolenoid, LOW);

  // launch the OLED screen
  launchSSD1306();

  // launch the LCD screen
  launchST7735S();

  // initalize the servo for the cooling press
  Servo cooling;
  cooling.attach(coolingServo);
  cooling.write(0);

  // initalize the servo for the heat press
  Servo heating; 
  heating.attach(heatingServo);
  heating.write(0);

  // create a timer that updates the sheetpress zones every 30 seconds
  timer.every(zoneUpdatePeriod, sheetpressHeatingPipeline);

}


// --- EXECUTE ---

void cookEnd() {

  // Update the system state
  cookState = OFF; 

  // Turn off all of the zones recieving power
  memset(recievingPower, 0, sizeof(recievingPower));
  switchSSRs();

  // Update the display
  displayActiveZones();
  clearDisplayTimerSelection();

}

void startCook() {

  // Change the system state
  cookState = ON;

  // Cook time is in seconds, so convert the units...
  unsigned long cookTimeInMiliseconds = (cookTime * 1000) * 60;
  unsigned long cookEndTimeInMiliseconds = millis() + cookTimeInMiliseconds; 

  // Add the cook timer as a concurrent task to the timer object.
  timer.in(cookTimeInMiliseconds, cookEnd);

  // Clear the display
  clearDisplayTimerSelection();

}

void loop() {

  // Check to see if the timer should execute the zone dchange pipeline
  timer.tick();

  // Apply logic based on the state of the timer
  switch (cookState) {

    case OFF:

      // Get knob input to interpret cook timer settings
      incrementCookTime();
      displayTimerSelection();

      // Confirm cook time by pressing the button
      if (buttonPressed()) { startCook(); }
      break;

    case ON:

      // Update the sheetpress display
      unsigned long remainingTimeMiliseconds = cookEndTimeInMiliseconds - millis(); 
      updateDisplayRemainingCookTime(remainingTimeMiliseconds);

      // Sleep until the next task to save compute resources
      unsigned long ticksUntilNextTask = timer.ticks();
      delay(ticksUntilNextTask);

      break;

  }

}
