# build-thing-sensors
<img src="/BC_SENSOR_SERIES/Extra/photo/Front_BC_Sensor_Serise.png" width=40%>
<a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>

This is an Arduino IDE library to BC Sensor Series

# Repository Contents
- /Examples - Example sketches for the library (.ino). Run these from the Arduino IDE.
- /Extra - Illust and photo. Ignored by IED
- /src - Source files for the library (.cpp, .h).

# Examples
- **BC_Seonsr_Series** - the BC Sensor Series(BCDPDC100, BCHTS4085, BCALS083K, BCACC316G, BCAQS1187) for Arduino. 
- **BCACC316G** - the BCACC316G Sensor(ADXL343_3axis accelerometer) for Arduino. 
- **BCALS083K** - the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Arduino.
- **BCAQS1187** - the BCAQS1187 Sensor(BCAQS1187_Indoor air quality sensor) for Arduino.
- **BCDPDC100** - the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for Arduino.
- **BCHTS4085** - the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for Arduino.

# Products that use this Library
- <a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>
- <a href="http://www.devicemart.co.kr/1384698" rel="nofollow">BCACC316G</a>
- <a href="http://www.devicemart.co.kr/1384700" rel="nofollow">BCALS083K</a>
- <a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCAQS1187</a>
- <a href="http://www.devicemart.co.kr/1384705" rel="nofollow">BCDPDC100</a>
- <a href="http://www.devicemart.co.kr/1384697" rel="nofollow">BCHTS4085</a>

# Typical Application
 <img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BC_Sensor_Series.png" width=40%>
 <a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>
<hr/>
<img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BCACC316G.png" width=40%>
<a href="http://www.devicemart.co.kr/1384698" rel="nofollow">BCACC316G</a>
<hr/>
<img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BCALS083K.png" width=40%>
<a href="http://www.devicemart.co.kr/1384700" rel="nofollow">BCALS083K</a>
<hr/>
<img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BCAQS1187.png" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCAQS1187</a>
<hr/>
<img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BCDPDC100.png" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCDPDC100</a>
<hr/>
<img src="/BC_SENSOR_SERIES/Extra/illust/Wire_BCHTS4085.png" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCHTS4085</a>
<hr/>

# How to Change Sensor Address (exclude BCDPDC100)

 - **Step 1** Remove

   Removing resistance on  Resistor selection field
   
 - **Step 2** Solder

   Solder the resistor(0 Ω) to the desired address
   
 - **Step 3** Edit Code

   Edit code the address value defined in the header file.
   For example.   

```
//#define BCHTS4085_ADDR 0x40 //HDC2010
#define BCHTS4085_ADDR 0x41 //HDC2010
```


# How to Use Library

 - **Step 1** Download
 
  Download the most recent version of the library
  
<p style="text-align:center;"><a href="https://github.com/buildit-lab/build-thing-sensors/archive/Dev_BC_Sensor_Series.zip" " class="btn btn-default"> the BC Sensor Series Library zip file</a></p>

- **Step 2** Select the example

  Select the example of the sensor module you own. And the library files are needed to be located in the same folder with the example files. 
  
  **BC_Seonsr_Series.ino** -BC_SENSORS.cpp, BC_SENSORS.h, BCACC316G.cpp, BCACC316G.h, BCALS083K.cpp, BCALS083K.h, BCAQS1187.cpp, BCAQS1187.h, BCDPDC100.cpp, BCDPDC100.h, BCHTS4085.cpp, BCHTS4085.h
                         
  **BCACC316G.ino** - BCACC316G.cpp, BCACC316G.h
  

  **BCALS083K.ino** - BCALS083K.cpp, BCALS083K.h
  
  
  **BCAQS1187.ino** - BCAQS1187.cpp, BCAQS1187.h
  
  
  **BCDPDC100.ino** - BCDPDC100.cpp, BCDPDC100.h
  
  
  **BCHTS4085.ino** - BCHTS4085.cpp, BCHTS4085.h
  
- **Step 3** Edit code

Feel free to start from the example sketch, or begin writing your own code using the functions provided by the library. In general, users  check out the example code to learn faster how to use the library. 


You’ll have to include the library, create a sensor object in the global space, and then use functions of that object to begin and control the sensor. With this one, pass the I2C address to the object during construction.

```
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

BC_SENSORS bc_sensors;    //use only BC_SENSOR_SERIES.ino

/*************************************************************************
* Declaration Sensors. 
* Select Declaration of the sensors, edit this section.
************************************************************************/  
  BCDPDC100 bcdpdc100 (BCDPDC100_ADDR); //TMD2671 Digital Proximity Detector
  BCHTS4085 bchts4085 (BCHTS4085_ADDR); //HDC2010 Temperature and Humidity Sensor
  BCALS083K bcals083k (BCALS083K_ADDR); //OPT3001Q1 Ambient Light Sensor
  BCACC316G bcacc316g (BCACC316G_ADDR); //ADXL343 3-Axis Accelerometer
  BCAQS1187 bcaqs1187 (BCAQS1187_ADDR); //CSS811 Indoor Air Quality Sensor 
```

To make the sensor get ready during program boot, wire.begin() must be called. (but BC_SENSOR_SERIES, bc_sensors.begin() ) 

```
void setup() {
  Serial.begin (115200);

  while (!Serial)
    {
    }
    
  Wire.begin();  
  sensorInitialize();
   
}  // end of setup
```

```
void setup() {
  Serial.begin (115200);

  while (!Serial)
    {
    }
    
  bc_sensors.begin();  
  sensorsInitialize();
   
}  // end of setup
```

 In the main loop of the program, call sensor functions such as *SensorName*.readResults() to operate the sensor. Check out the examples for fully functional code.
 
 Check the sensorsInitialize (void) function to set and initialize the sensor before reading it, Check the readSensorsData (void) function for reading and manipulating sensor data.

Function references can be found in each sensor library header file. For usage, see the readSensorsData () function in each example.

<img src="/BC_SENSOR_SERIES/Extra/illust/SERIAL_MONITOR_BC_SENSOR_SERIES.png" width=60%>

<hr/>
