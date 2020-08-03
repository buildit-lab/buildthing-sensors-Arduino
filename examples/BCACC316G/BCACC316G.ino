#define BCACC316G_ADDR 0x53 //ADXL343

#define ALERTPIN1 2
#define ALERTPIN2 3

#include "BCACC316G.h"
#include <Wire.h>

void sensorInitialize(void);
void readSensorData(void);

BCACC316G bcacc316g (BCACC316G_ADDR); //ADXL343 3-Axis Accelerometer

void setup() {
  Serial.begin (115200);

  while (!Serial)
    {
    }
    
  Wire.begin();  
  sensorInitialize();
   
}  // end of setup


void loop() {  
  readSensorData();
  Serial.println();
  delay(1000);
}

void sensorInitialize(void){
  Serial.println("[BCACC316G] 3axis accletormeter Enabled");
  bcacc316g.powerOn();
  
  //set activity/ inactivity thresholds (0-255)
  bcacc316g.setActivityThreshold(75); //62.5mg per increment
  bcacc316g.setInactivityThreshold(75); //62.5mg per increment
  bcacc316g.setTimeInactivity(10); // how many seconds of no activity is inactive?
  
  //look of activity movement on this axes - 1 == on; 0 == off 
  bcacc316g.setActivityX(1);
  bcacc316g.setActivityY(1);
  bcacc316g.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  bcacc316g.setInactivityX(1);
  bcacc316g.setInactivityY(1);
  bcacc316g.setInactivityZ(1);
  
  //look of tap movement on this axes - 1 == on; 0 == off
  bcacc316g.setTapDetectionOnX(0);
  bcacc316g.setTapDetectionOnY(0);
  bcacc316g.setTapDetectionOnZ(1);
  
  //set values for what is a tap, and what is a double tap (0-255)
  bcacc316g.setTapThreshold(50); //62.5mg per increment
  bcacc316g.setTapDuration(15); //625μs per increment
  bcacc316g.setDoubleTapLatency(80); //1.25ms per increment
  bcacc316g.setDoubleTapWindow(200); //1.25ms per increment
  
  //set values for what is considered freefall (0-255)
  bcacc316g.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  bcacc316g.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
  
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  bcacc316g.setInterruptMapping( BCACC316G_INT_SINGLE_TAP_BIT, BCACC316G_INT1_PIN );
  bcacc316g.setInterruptMapping( BCACC316G_INT_DOUBLE_TAP_BIT, BCACC316G_INT1_PIN );
  bcacc316g.setInterruptMapping( BCACC316G_INT_FREE_FALL_BIT, BCACC316G_INT1_PIN );
  bcacc316g.setInterruptMapping( BCACC316G_INT_ACTIVITY_BIT, BCACC316G_INT1_PIN );
  bcacc316g.setInterruptMapping( BCACC316G_INT_INACTIVITY_BIT, BCACC316G_INT1_PIN );
  
  //register interupt actions - 1 == on; 0 == off 
  bcacc316g.setInterrupt( BCACC316G_INT_SINGLE_TAP_BIT, 1);
  bcacc316g.setInterrupt( BCACC316G_INT_DOUBLE_TAP_BIT, 1);
  bcacc316g.setInterrupt( BCACC316G_INT_FREE_FALL_BIT, 1);
  bcacc316g.setInterrupt( BCACC316G_INT_ACTIVITY_BIT, 1);
  bcacc316g.setInterrupt( BCACC316G_INT_INACTIVITY_BIT, 1);
 
  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorData()
{  
  int x,y,z; 
  bcacc316g.readAccel(&x, &y, &z); //read the accelerometer values and store them in variables x,y,z
  
  // Output x,y,z values - Commented out
  Serial.print("[BCACC316G] x : ");Serial.print(x);
  Serial.print("  y : ");Serial.print(y);
  Serial.print("  z : ");Serial.print(z);


  //Fun Stuff! 
  //read interrupts source and look for triggerd actions
  
  //getInterruptSource clears all triggered actions after returning value
  //so do not call again until you need to recheck for triggered actions
  byte interrupts = bcacc316g.getInterruptSource();
  
  // freefall
  if(bcacc316g.triggered(interrupts, BCACC316G_FREE_FALL))
  {
      Serial.print(" (freefall) ");
  } 
  
  //inactivity
  if(bcacc316g.triggered(interrupts, BCACC316G_INACTIVITY))
  {
      Serial.print(" (inactivity) ");
  }
  
  //activity
  if(bcacc316g.triggered(interrupts, BCACC316G_ACTIVITY))
  {
      Serial.print(" (activity) "); 
  }
  
  //double tap
  if(bcacc316g.triggered(interrupts, BCACC316G_DOUBLE_TAP))
  {
      Serial.print(" (double tap) ");
  }
  
  //tap
  if(bcacc316g.triggered(interrupts, BCACC316G_SINGLE_TAP))
  {
      Serial.print("(tap)");
  }
  Serial.println();
}
 
