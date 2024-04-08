#include "rotary_encoder.h"

void buttonTurning(){
  // Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter ++;

		} else {
			// Encoder is rotating CW so increment
			counter --;

		}

	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

}


void incrementCookTime() {

  // Get input from the turning encoder
  buttonTurning();

  // Based on the position of the encoder, change the selected cook time
  int cookTime = defaultCookTime + cookTimeIncriment * counter; 

}


bool buttonPressed() {

  return digitalRead(SW);

}