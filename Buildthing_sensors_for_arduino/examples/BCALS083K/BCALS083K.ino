#define BCALS083K_ADDR 0x44 //OPT3001Q1


#define ALERTPIN 6

#include "BCALS083K.h"
#include <Wire.h>

BCALS083K bcals083k (BCALS083K_ADDR); //OPT3001Q1 Ambient Light Sensor

void sensorInitialize(void);
void readSensorData(void);

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
  Serial.println("[BCALS083K] Ambient Light Sensor Enabled");

  bcals083k.setRange(12); //Auto full scale
  bcals083k.setTime(1);   //set time 800ms
  bcals083k.setMode(COUTINUOUS);
  bcals083k.setPolarity(ACTIVE_HIGH);
  bcals083k.setLatch(WINDOW_STYLE);
  bcals083k.setMaskExponent(1); //Range set to less than 14
  bcals083k.setFaultCount(FAULT_CNT_ONE);

  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorData()
{  
  Serial.print("[BCALS083K] ALS : ");
  Serial.print(bcals083k.readResult());
  Serial.println(" lux");
}
 
