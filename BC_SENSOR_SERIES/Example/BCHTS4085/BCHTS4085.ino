#define BCHTS4085_ADDR 0x40 //HDC2010

#define ALERTPIN 7

#include <Wire.h>
#include "BCHTS4085.h"  //HDC2010 Temperature and Humidity Sensor

void sensorInitialize(void);
void readSensorData(void);

BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor

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
  Serial.println("[BCHTS4085] Temperature & Humidity Sensor Enabled");
    
  bchts4085.setTempRes(FOURTEEN_BIT);
  bchts4085.setHumidRes(FOURTEEN_BIT);
  bchts4085.setRate(ONE_HZ);
  bchts4085.triggerMeasurement();

  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorData()
{  
  Serial.print("[BCHTS4085] Temp : ");
  Serial.print(bchts4085.readTemp());
  Serial.print(" ℃   Humid : ");
  Serial.print(bchts4085.readHumidity());         
  Serial.println(" %");
}
 
