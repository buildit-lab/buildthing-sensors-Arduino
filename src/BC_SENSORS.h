/*
  BC_SENSORS.h
  Created By: sy.baik, 2018-05-25
  
  This library facilitates communication with, and configuration of, 
  the BC Sensor Series(BCDPDC100, BCHTS4085, BCALS083K, BCACC316G, BCAQS1187) for Arduino.  
*/

#ifndef BC_SENSORS_H
#define BC_SENSORS_h

/*************************************************************************
 * Sensors defalut address. 
 * If you want to change the address of the sensors, modify this section.
 ************************************************************************/
  #define BCDPDC100_ADDR 0x39 //TMD2671 Digital Proximity Detector
  #define BCHTS4085_ADDR 0x40 //HDC2010 Temperature and Humidity Sensor
  #define BCALS083K_ADDR 0x44 //OPT3001Q1 Ambient Light Sensor
  #define BCACC316G_ADDR 0x53 //ADXL343 3-Axis Accelerometer
  #define BCAQS1187_ADDR 0x5A //CSS811 Indoor Air Quality Sensor
/************************************************************************/

/*************************************************************************
 * Select include files. 
 * Select include file of the sensors, modify this section.
 ************************************************************************/
  #include "BCDPDC100.h" //TMD2671 Digital Proximity Detector
  #include "BCHTS4085.h"  //HDC2010 Temperature and Humidity Sensor
  #include "BCALS083K.h"  //OPT3001Q1 Ambient Light Sensor
  #include "BCACC316G.h"  //ADXL343 3-Axis Accelerometer
  #include "BCAQS1187.h"  //CSS811  Indoor Air Quality Sensor
/************************************************************************/

#include <Arduino.h>
#include <Wire.h>


class BC_SENSORS
{
  public:
    void begin(void);      
};
#endif
