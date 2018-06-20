/*
  BC_SENSORS.cpp
  Created By: sy.baik, 2018-05-25
  
  This library facilitates communication with, and configuration of, 
  the BC Sensor Series(BCDPDC100, BCHTS4085, BCALS083K, BCACC316G, BCAQS1187) for Arduino.  
*/


#include "BC_SENSORS.h"
#include <Wire.h>

void BC_SENSORS::begin(void)
{
  Wire.begin();
}





