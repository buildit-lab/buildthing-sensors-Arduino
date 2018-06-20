#define BCDPDC100_ADDR 0x39 //TMD2671

#define ALERTPIN 5

#include "BCDPDC100.h"
#include <Wire.h>

void sensorInitialize(void);
void readSensorData(void);

BCDPDC100 bcdpdc100 (BCDPDC100_ADDR); //TMD2671 Digital Proximity Detector

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
  Serial.println("[BCDPDC100] Digital Proximity Detector Enabled");
  
  bcdpdc100.powerOn();
  bcdpdc100.enableInterruptMask();
  bcdpdc100.enableWait();    
  bcdpdc100.disableProximity();
  bcdpdc100.writeAtime(0xFF);
  bcdpdc100.writePtime(0xFF);
  bcdpdc100.writeWtime(0xFF);
  bcdpdc100.setLedPower(BCDPDC100_LED100P);
  bcdpdc100.setDiode(BCDPDC100_BOTH);
  bcdpdc100.setPpulse(32);    
  bcdpdc100.setLowThresold(0);    
  bcdpdc100.setHighThresold(1000);  
  bcdpdc100.setPersistence(10);
  bcdpdc100.enableProximity();    
  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorData()
{  
  Serial.print("[BCDPDC100] Proximity ADC : ");
  Serial.print(bcdpdc100.readProximityAdc());
    
        
  //Proximity inturrupt
 if(bcdpdc100.readInterrupt())
  {
    Serial.print(" (proximity) ");
  }
    Serial.println();
}
 
