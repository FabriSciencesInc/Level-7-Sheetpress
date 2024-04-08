#ifndef _ZONE_CONTROL_H_
#define _ZONE_CONTROL_H_

#include <Arduino.h>
#include "globals.h"

void fillPowerRequestVector();
void findNextActiveZones();
void switchSSRs();

#endif