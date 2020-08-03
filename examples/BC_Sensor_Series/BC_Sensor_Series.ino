#define BCDPDC100_ADDR 0x39 //TMD2671
#define BCHTS4085_ADDR 0x40 //HDC2010
#define BCALS083K_ADDR 0x44 //OPT3001Q1
#define BCACC316G_ADDR 0x53 //ADXL343
#define BCAQS1187_ADDR 0x5A  //CSS811

#define BCACC316G_INT1 2
#define BCACC316G_INT2 3
#define BCAQS1187_INT 4
#define BCDPDC100_INT 5
#define BCALS083K_INT 6
#define BCHTS4085_INT 7


#include "BC_SENSORS.h"
#include <Wire.h>

void sensorsInitialize(void);
void readSensorsData(void);

BC_SENSORS bc_sensors;

/*************************************************************************
* Declaration Sensors. 
* Select Declaration of the sensors, modify this section.
************************************************************************/  
  BCDPDC100 bcdpdc100 (BCDPDC100_ADDR); //TMD2671 Digital Proximity Detector
  BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor
  BCALS083K bcals083k (BCALS083K_ADDR); //OPT3001Q1 Ambient Light Sensor
  BCACC316G bcacc316g (BCACC316G_ADDR); //ADXL343 3-Axis Accelerometer
  BCAQS1187 bcaqs1187 (BCAQS1187_ADDR); //CSS811 Indoor Air Quality Sensor 
/*************************************************************************
* To use the sensors, change the value to 'true' or not 'false'
*************************************************************************/
boolean bcdpdc100_En=true;
boolean bchts4085_En=true;
boolean bcals083k_En=true;
boolean bcacc316g_En=true;
boolean bcaqs1187_En=true;
/************************************************************************/

  

void setup() {
  Serial.begin (115200);

  while (!Serial)
    {
    }
    
  bc_sensors.begin();  
  sensorsInitialize();
   
}  // end of setup


void loop() {  
  readSensorsData();
  Serial.println();
  delay(1000);
}

void sensorsInitialize(void){
  if(bcdpdc100_En)
  { 
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
  }
  if(bchts4085_En)
  { 
    Serial.println("[BCHTS4085] Temperature & Humidity Sensor Enabled");
    
    bchts4085.setTempRes(FOURTEEN_BIT);
    bchts4085.setHumidRes(FOURTEEN_BIT);
    bchts4085.setRate(ONE_HZ);
    bchts4085.triggerMeasurement();
  }
  if(bcals083k_En)
  {
    Serial.println("[BCALS083K] Ambient Light Sensor Enabled");

    bcals083k.setRange(12); //Auto full scale
    bcals083k.setTime(1);   //set time 800ms
    bcals083k.setMode(COUTINUOUS);
    bcals083k.setPolarity(ACTIVE_HIGH);
    bcals083k.setLatch(WINDOW_STYLE);
    bcals083k.setMaskExponent(1); //Range set to less than 14
    bcals083k.setFaultCount(FAULT_CNT_ONE);
  }
  if(bcacc316g_En)
  {
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
  }
  if(bcaqs1187_En)
  {
    Serial.println("[BCAQS1187] Indoor Air Quarity Sensor Enabled");
    bcaqs1187.SWReset();
    delay(100);
    bcaqs1187.appStart();
    bcaqs1187.setThresholds((uint16_t) 1500, (uint16_t) 2500, (uint8_t) 50);
    bcaqs1187.setDriveMode(BCAQS1187_DRIVE_MODE_1SEC); 
  }  
  
  Serial.println("Initialized BC Sensors Series");  
  Serial.println();
  Serial.println();
}

void readSensorsData()
{  
  if(bcdpdc100_En)
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
  if(bchts4085_En)
  {     
    Serial.print("[BCHTS4085] Temp : ");
    Serial.print(bchts4085.readTemp());
    Serial.print(" ℃   Humid : ");
    Serial.print(bchts4085.readHumidity());         
    Serial.println(" %");
  }
  if(bcals083k_En)
  {
    Serial.print("[BCALS083K] ALS : ");
    Serial.print(bcals083k.readResult());
    Serial.println(" lux");
  }
  if(bcacc316g_En)
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
  if(bcaqs1187_En)
  {
    bcaqs1187.readResultData();
    Serial.print("[BCAQS1187] TVOC : ");
    Serial.print(bcaqs1187.getTVOC());
    Serial.print(" ppb   eCO2 : ");
    Serial.print(bcaqs1187.geteCO2());  
    Serial.println(" ppm");
  }  
}
 
