# build-thing-sensors
<img src="/Buildthing_sensors_for_ARM/Extra/photo/Front_BC_Sensor_Serise.png" width=40%>
<a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>

This is an Keil uVision5 library using stm32fvgt6x microcontroller to BC Sensor Series
***If you are using a different processor, you must check its pin connection.***

# Library Tested Conditions
- **Processor** : STM32F4VGT6
- **UTILITY** : STM32CubeMX, HAL Library
- **Integrated Development Environment(IDE)** : Keil uVision5

# Repository Contents
- /Examples - Example project file for the library (/MDK-ARM/(file_name).uvprojx). Run these from the Keil.
- /Extra - Illust and photo.
- /Inc - Header files for the library (.h).
- /Src - Sorce files for the library (.c).

# Example
- **BC_Sensor_Series** - the BC Sensor Series(BCDPDC100, BCHTS4085, BCALS083K, BCACC316G, BCAQS1187) for Keil. 
- **BCACC316G** - the BCACC316G Sensor(ADXL343_3axis accelerometer) for Keil. 
- **BCALS083K** - the BCALS083K Sensor(OPT3001_Q1_Ambient Light Sensor) for Keil.
- **BCAQS1187** - the BCAQS1187 Sensor(CSS811_Air Quality Sensor) for Keil.
- **BCDPDC100** - the BCDPDC100 Sensor(TMD2671_Digital Proximity Detector) for Keil.
- **BCHTS4085** - the BCHTS4085 Sensor(HDC2010_Temperature and Humidity Sensor) for Keil.

# Products that use this Library
- <a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>
- <a href="http://www.devicemart.co.kr/1384698" rel="nofollow">BCACC316G</a>
- <a href="http://www.devicemart.co.kr/1384700" rel="nofollow">BCALS083K</a>
- <a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCAQS1187</a>
- <a href="http://www.devicemart.co.kr/1384705" rel="nofollow">BCDPDC100</a>
- <a href="http://www.devicemart.co.kr/1384697" rel="nofollow">BCHTS4085</a>
- <a href="http://www.devicemart.co.kr/1376634" rel="nofollow">USB to Serial(replaceable)</a>

# Typical Application
 <img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BC_Sensor_Series.png" width=40%>
 <a href="http://www.devicemart.co.kr/1384726" rel="nofollow">BC_Sensor_Series</a>
<hr/>
<img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BCACC316G.PNG" width=40%>
<a href="http://www.devicemart.co.kr/1384698" rel="nofollow">BCACC316G</a>
<hr/>
<img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BCALS083K.PNG" width=40%>
<a href="http://www.devicemart.co.kr/1384700" rel="nofollow">BCALS083K</a>
<hr/>
<img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BCAQS1187.PNG" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCAQS1187</a>
<hr/>
<img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BCDPDC100.PNG" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCDPDC100</a>
<hr/>
<img src="/Buildthing_sensors_for_ARM/Extra/illust/Wire_BCHTS4085.PNG" width=40%>
<a href="http://www.devicemart.co.kr/1384706" rel="nofollow">BCHTS4085</a>
<hr/>

# How to Change Sensor Address (exclude BCDPDC100)

 - **Step 1** Remove

   Removing resistance on  Resistor selection field
   
 - **Step 2** Solder

   Solder the resistor(0 Ω) to the desired address
   
 - **Step 3** Edit Code

   Edit code the address value defined in .uvprojx file.
   For example.   

    ```
    //#define BCHTS4085_ADDR 0x40 //HDC2010
    #define BCHTS4085_ADDR 0x41 //HDC2010
    ```


# How to Use Library

See ***Library Tested Conditions*** and check your development environment. Then, Please follow these steps.

- **Step 1** Download
 
  Download the most recent version of the library from our git site
  
 - **Step 2** Check your circuit
  
    We used UART3 pin(***PC10***-rx, ***PC11***-tx), I2C pin(***PB10***-SCL, ***PB11***-SDA). Then, Check your own processor datasheet and change your circuit, source code. 
 
- **Step 3** Select the example 
    
   Select the example of the sensor module you own. Project file path is ***/Examples/(sensorName)/MDK-ARM/(sensorName).uvprojx***

- **Step 4** Build the example

    Build target files and Download code to flash memory. You can get your sensor data by using any terminal program.
    <img src="/Buildthing_sensors_for_ARM/Extra/photo/Serial_Data.PNG" width=40%>

    



















