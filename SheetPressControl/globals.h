
#ifndef _GLOBALS_
#define _GLOBALS_


// --- DEFINE GLOBAL STATE PARAMETERS ---
enum CookState { OFF, ON };
extern CookState cookState; 


// --- DEFINE GLOBAL COMPUTE PARAMETERS ---

// Pins for heater input and output
extern const int PID_0, PID_1, PID_2, PID_3, PID_4, PID_5, Zone_0, Zone_1, Zone_2, Zone_3, Zone_4, Zone_5; 

// Pins for solenoid activation
extern const int coolingSolenoid, heatingSolenoid;

// Pins for servo
extern const int coolingServo, heatingServo; 

// Pins for knob
extern const int CLK, DT, SW; 

// Pins for knob control
extern int counter, currentStateCLK, lastStateCLK;

// Array to hold PID power request information
extern int requestingPower[6];

// Array to know which zones are recieving power
extern int recievingPower[6];

// Array to store which zones are active
extern int activeZones[2];

// Keep track of if the two possible power zones are actually on
extern bool powerNeeded1, powerNeeded2;

// Variables to store selected cook time (in minute units)
extern int defaultCookTime;
extern int cookTimeIncriment; 
extern int cookTime; 


// --- DEFINE GLOBAL RENDER PARAMETERS ---

// Variables for Level 7 logo
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    60

// Varables for SSD1306
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 

#endif