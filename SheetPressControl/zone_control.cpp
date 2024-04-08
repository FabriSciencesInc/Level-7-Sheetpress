// --- HELPER FUNCTIONS FOR TEMPERATURE CONTROL ---
#include "zone_control.h"

// populate vector defining zones that require power
void fillPowerRequestVector() {

  requestingPower[0] = digitalRead(PID_0);
  requestingPower[1] = digitalRead(PID_1);
  requestingPower[2] = digitalRead(PID_2);
  requestingPower[3] = digitalRead(PID_3);
  requestingPower[4] = digitalRead(PID_4);
  requestingPower[5] = digitalRead(PID_5);

}

// determine which zones should recieve power based on power requests
void findNextActiveZones() {

  // determine which zones are active
  int count = 0;
  for(int i = 0; i < 6; i++){
    if(recievingPower[i] != 0){
      if(count == 0){
        activeZones[0] = i;
        activeZones[1] = i;
        count++;
      } else {
        activeZones[1] = i;
      }
    }
  }

  // sort active zones in acending order for easier compute
  if(activeZones[0] > activeZones[1]){
    int temp = activeZones[0];
    activeZones[0] = activeZones[1];
    activeZones[1] = temp;
  }

  // FIND ZONES TO PROVIDE POWER

  int newZoneIndex1 = 0;
  int newZoneIndex2 = 0;
  bool powerNeeded1 = false;
  bool powerNeeded2 = false; 

  // Reset power source 1
  for(int i = 1; i < 7; i++){
    newZoneIndex1 = (activeZones[0] + i) % 6;
    if(requestingPower[newZoneIndex1] == 0) continue;
    if(newZoneIndex1 == activeZones[1]) continue;
    powerNeeded1 = true;
    break;
  }

  // Reset power source 2
  for(int i = 1; i < 7; i++){
    newZoneIndex2 = (activeZones[1] + i) % 6;
    if(requestingPower[newZoneIndex2] == 0) continue;
    if(powerNeeded1 == true && newZoneIndex2 == newZoneIndex1) continue;
    powerNeeded2 = true;
    break;
  }

  // WRITE DATA TO OUTPUT VECTOR

  // set all values in recieving power array to zero
  memset(recievingPower, 0, sizeof(recievingPower));

  // activate zones in recieving power array to correspond with zones with power delivery
  if(powerNeeded1 && powerNeeded2){
    recievingPower[newZoneIndex1] = 1;
    recievingPower[newZoneIndex2] = 1;
  } else if(powerNeeded1){
    recievingPower[newZoneIndex1] = 1;
  } else if(powerNeeded2){
    recievingPower[newZoneIndex2] = 1;
  }
  
}

// turn required PIDs on/off
void switchSSRs() {

  digitalWrite(Zone_0, recievingPower[0]);
  digitalWrite(Zone_1, recievingPower[1]);
  digitalWrite(Zone_2, recievingPower[2]);
  digitalWrite(Zone_3, recievingPower[3]);
  digitalWrite(Zone_4, recievingPower[4]);
  digitalWrite(Zone_5, recievingPower[5]);

}