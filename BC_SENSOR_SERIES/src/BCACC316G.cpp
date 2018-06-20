/*
  BCACC316G.c
  Created By: sy.baik, 2018-05-27
  
  This library facilitates communication with, and configuration of, 
  the BCACC316G Sensor(ADXL343_3axis accelerometer) for Arduino. 
*/

#include <Wire.h>

#include "BCACC316G.h"

#define BCACC316G_TO_READ (6) // num of bytes we are going to read each time (two bytes for each axis)

BCACC316G::BCACC316G(uint8_t addr) 
{
  _addr=addr;
  status = BCACC316G_OK;
  error_code = BCACC316G_NO_ERROR;

  gains[0] = 0.00376390;
  gains[1] = 0.00376009;
  gains[2] = 0.00349265;
}

void BCACC316G::powerOn() 
{
  writeTo(BCACC316G_POWER_CTL, 0); 
  writeTo(BCACC316G_POWER_CTL, 16);
  writeTo(BCACC316G_POWER_CTL, 8); 
}

// Reads the acceleration into three variable x, y and z
void BCACC316G::readAccel(int *xyz)
{
  readAccel(xyz, xyz + 1, xyz + 2);
}
void BCACC316G::readAccel(int *x, int *y, int *z) 
{
  readFrom(BCACC316G_DATAX0, BCACC316G_TO_READ, _buff); //read the acceleration data from the BCACC316G

  // each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  *x = (((int)_buff[1]) << 8) | _buff[0]; 
  *y = (((int)_buff[3]) << 8) | _buff[2];
  *z = (((int)_buff[5]) << 8) | _buff[4];
}

void BCACC316G::get_Gxyz(double *xyz)
{
  int i;
  int xyz_int[3];
  readAccel(xyz_int);
  for(i=0; i<3; i++){
    xyz[i] = xyz_int[i] * gains[i];
  }
}

// Writes val to address register on device
void BCACC316G::writeTo(byte address, byte val) 
{
  Wire.beginTransmission(_addr); // start transmission to device 
  Wire.write(address); // send register address
  Wire.write(val); // send value to write
  Wire.endTransmission(); // end transmission
}

// Reads num bytes starting from address register on device in to _buff array
void BCACC316G::readFrom(byte address, int num, byte _buff[]) 
{
  Wire.beginTransmission(_addr); // start transmission to device 
  Wire.write(address); // sends address to read from
  Wire.endTransmission(); // end transmission

  Wire.beginTransmission(_addr); // start transmission to device
  Wire.requestFrom(_addr, num); // request 6 bytes from device

  int i = 0;
  while(Wire.available()) // device may send less than requested (abnormal)
  { 
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }
  if(i != num)
  {
    status = BCACC316G_ERROR;
    error_code = BCACC316G_READ_ERROR;
  }  
  Wire.endTransmission(); // end transmission
 }

// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void BCACC316G::getRangeSetting(byte* rangeSetting) 
{
  byte _b;
  readFrom(BCACC316G_DATA_FORMAT, 1, &_b);
  *rangeSetting = _b & B00000011;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void BCACC316G::setRangeSetting(int val) 
{
  byte _s;
  byte _b;

  switch (val) {
    case 2: 
      _s = B00000000; 
    break;
    
    case 4: 
      _s = B00000001; 
    break;
    
    case 8: 
      _s = B00000010; 
    break;
    
    case 16: 
      _s = B00000011; 
    break;
    
    default: 
      _s = B00000000;
    break;
  }
  readFrom(BCACC316G_DATA_FORMAT, 1, &_b);
  _s |= (_b & B11101100);
  writeTo(BCACC316G_DATA_FORMAT, _s);
}

// gets the state of the SELF_TEST bit
bool BCACC316G::getSelfTestBit() 
{
  return getRegisterBit(BCACC316G_DATA_FORMAT, 7);
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void BCACC316G::setSelfTestBit(bool selfTestBit) 
{
  setRegisterBit(BCACC316G_DATA_FORMAT, 7, selfTestBit);
}

// Gets the state of the SPI bit
bool BCACC316G::getSpiBit() 
{
  return getRegisterBit(BCACC316G_DATA_FORMAT, 6);
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void BCACC316G::setSpiBit(bool spiBit) 
{
  setRegisterBit(BCACC316G_DATA_FORMAT, 6, spiBit);
}

// Gets the state of the INT_INVERT bit
bool BCACC316G::getInterruptLevelBit() 
{
  return getRegisterBit(BCACC316G_DATA_FORMAT, 5);
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void BCACC316G::setInterruptLevelBit(bool interruptLevelBit) 
{
  setRegisterBit(BCACC316G_DATA_FORMAT, 5, interruptLevelBit);
}

// Gets the state of the FULL_RES bit
bool BCACC316G::getFullResBit() 
{
  return getRegisterBit(BCACC316G_DATA_FORMAT, 3);
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
// g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
// and scale factor
void BCACC316G::setFullResBit(bool fullResBit) 
{
  setRegisterBit(BCACC316G_DATA_FORMAT, 3, fullResBit);
}

// Gets the state of the justify bit
bool BCACC316G::getJustifyBit() 
{
  return getRegisterBit(BCACC316G_DATA_FORMAT, 2);
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void BCACC316G::setJustifyBit(bool justifyBit) 
{
  setRegisterBit(BCACC316G_DATA_FORMAT, 2, justifyBit);
}

// Sets the THRESH_TAP byte value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void BCACC316G::setTapThreshold(int tapThreshold) 
{
  tapThreshold = constrain(tapThreshold,0,255);
  byte _b = byte (tapThreshold);
  writeTo(BCACC316G_THRESH_TAP, _b); 
}

// Gets the THRESH_TAP byte value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int BCACC316G::getTapThreshold() 
{
  byte _b;
  readFrom(BCACC316G_THRESH_TAP, 1, &_b); 
  return int (_b);
}

// set/get the gain for each axis in Gs / count
void BCACC316G::setAxisGains(double *_gains)
{
  int i;
  for(i = 0; i < 3; i++){
    gains[i] = _gains[i];
  }
}
void BCACC316G::getAxisGains(double *_gains)
{
  int i;
  for(i = 0; i < 3; i++){
    _gains[i] = gains[i];
  }
}


// Sets the OFSX, OFSY and OFSZ bytes
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between 
void BCACC316G::setAxisOffset(int x, int y, int z) 
{
  writeTo(BCACC316G_OFSX, byte (x)); 
  writeTo(BCACC316G_OFSY, byte (y)); 
  writeTo(BCACC316G_OFSZ, byte (z)); 
}

// Gets the OFSX, OFSY and OFSZ bytes
void BCACC316G::getAxisOffset(int* x, int* y, int*z) 
{
  byte _b;
  readFrom(BCACC316G_OFSX, 1, &_b); 
  *x = int (_b);
  readFrom(BCACC316G_OFSY, 1, &_b); 
  *y = int (_b);
  readFrom(BCACC316G_OFSZ, 1, &_b); 
  *z = int (_b);
}

// Sets the DUR byte
// The DUR byte contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void BCACC316G::setTapDuration(int tapDuration) 
{
  tapDuration = constrain(tapDuration,0,255);
  byte _b = byte (tapDuration);
  writeTo(BCACC316G_DUR, _b); 
}

// Gets the DUR byte
int BCACC316G::getTapDuration() 
{
  byte _b;
  readFrom(BCACC316G_DUR, 1, &_b); 
  return int (_b);
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void BCACC316G::setDoubleTapLatency(int doubleTapLatency) 
{
  byte _b = byte (doubleTapLatency);
  writeTo(BCACC316G_LATENT, _b); 
}

// Gets the Latent value
int BCACC316G::getDoubleTapLatency() 
{
  byte _b;
  readFrom(BCACC316G_LATENT, 1, &_b); 
  return int (_b);
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void BCACC316G::setDoubleTapWindow(int doubleTapWindow) 
{
  doubleTapWindow = constrain(doubleTapWindow,0,255);
  byte _b = byte (doubleTapWindow);
  writeTo(BCACC316G_WINDOW, _b); 
}

// Gets the Window register
int BCACC316G::getDoubleTapWindow() 
{
  byte _b;
  readFrom(BCACC316G_WINDOW, 1, &_b); 
  return int (_b);
}

// Sets the THRESH_ACT byte which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared 
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// activity interrupt is enabled. The maximum value is 255.
void BCACC316G::setActivityThreshold(int activityThreshold) 
{
  activityThreshold = constrain(activityThreshold,0,255);
  byte _b = byte (activityThreshold);
  writeTo(BCACC316G_THRESH_ACT, _b); 
}

// Gets the THRESH_ACT byte
int BCACC316G::getActivityThreshold() 
{
  byte _b;
  readFrom(BCACC316G_THRESH_ACT, 1, &_b); 
  return int (_b);
}

// Sets the THRESH_INACT byte which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared 
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the 
// inactivity interrupt is enabled. The maximum value is 255.
void BCACC316G::setInactivityThreshold(int inactivityThreshold) 
{
  inactivityThreshold = constrain(inactivityThreshold,0,255);
  byte _b = byte (inactivityThreshold);
  writeTo(BCACC316G_THRESH_INACT, _b); 
}

// Gets the THRESH_INACT byte
int BCACC316G::getInactivityThreshold() 
{
  byte _b;
  readFrom(BCACC316G_THRESH_INACT, 1, &_b); 
  return int (_b);
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void BCACC316G::setTimeInactivity(int timeInactivity) 
{
  timeInactivity = constrain(timeInactivity,0,255);
  byte _b = byte (timeInactivity);
  writeTo(BCACC316G_TIME_INACT, _b); 
}

// Gets the TIME_INACT register
int BCACC316G::getTimeInactivity() 
{
  byte _b;
  readFrom(BCACC316G_TIME_INACT, 1, &_b); 
  return int (_b);
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The 
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void BCACC316G::setFreeFallThreshold(int freeFallThreshold) 
{
  freeFallThreshold = constrain(freeFallThreshold,0,255);
  byte _b = byte (freeFallThreshold);
  writeTo(BCACC316G_THRESH_FF, _b); 
}

// Gets the THRESH_FF register.
int BCACC316G::getFreeFallThreshold() 
{
  byte _b;
  readFrom(BCACC316G_THRESH_FF, 1, &_b); 
  return int (_b);
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall 
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void BCACC316G::setFreeFallDuration(int freeFallDuration) 
{
  freeFallDuration = constrain(freeFallDuration,0,255); 
  byte _b = byte (freeFallDuration);
  writeTo(BCACC316G_TIME_FF, _b); 
}

// Gets the TIME_FF register.
int BCACC316G::getFreeFallDuration() 
{
  byte _b;
  readFrom(BCACC316G_TIME_FF, 1, &_b); 
  return int (_b);
}

bool BCACC316G::isActivityXEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 6); 
}

bool BCACC316G::isActivityYEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 5); 
}

bool BCACC316G::isActivityZEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 4); 
}

bool BCACC316G::isInactivityXEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 2); 
}

bool BCACC316G::isInactivityYEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 1); 
}

bool BCACC316G::isInactivityZEnabled() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 0); 
}

void BCACC316G::setActivityX(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 6, state); 
}

void BCACC316G::setActivityY(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 5, state); 
}

void BCACC316G::setActivityZ(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 4, state); 
}

void BCACC316G::setInactivityX(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 2, state); 
}

void BCACC316G::setInactivityY(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 1, state); 
}

void BCACC316G::setInactivityZ(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 0, state); 
}

bool BCACC316G::isActivityAc() 
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 7); 
}

bool BCACC316G::isInactivityAc()
{ 
  return getRegisterBit(BCACC316G_ACT_INACT_CTL, 3); 
}

void BCACC316G::setActivityAc(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 7, state); 
}

void BCACC316G::setInactivityAc(bool state) 
{ 
  setRegisterBit(BCACC316G_ACT_INACT_CTL, 3, state); 
}

bool BCACC316G::getSuppressBit()
{ 
  return getRegisterBit(BCACC316G_TAP_AXES, 3); 
}

void BCACC316G::setSuppressBit(bool state) 
{ 
  setRegisterBit(BCACC316G_TAP_AXES, 3, state); 
}

bool BCACC316G::isTapDetectionOnX()
{ 
  return getRegisterBit(BCACC316G_TAP_AXES, 2); 
}

void BCACC316G::setTapDetectionOnX(bool state) 
{ 
  setRegisterBit(BCACC316G_TAP_AXES, 2, state); 
}

bool BCACC316G::isTapDetectionOnY()
{ 
  return getRegisterBit(BCACC316G_TAP_AXES, 1); 
}

void BCACC316G::setTapDetectionOnY(bool state) 
{ 
  setRegisterBit(BCACC316G_TAP_AXES, 1, state); 
}

bool BCACC316G::isTapDetectionOnZ()
{ 
  return getRegisterBit(BCACC316G_TAP_AXES, 0); 
}

void BCACC316G::setTapDetectionOnZ(bool state) 
{ 
  setRegisterBit(BCACC316G_TAP_AXES, 0, state); 
}

bool BCACC316G::isActivitySourceOnX()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 6); 
}

bool BCACC316G::isActivitySourceOnY()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 5); 
}

bool BCACC316G::isActivitySourceOnZ()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 4); 
}

bool BCACC316G::isTapSourceOnX()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 2); 
}

bool BCACC316G::isTapSourceOnY()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 1); 
}

bool BCACC316G::isTapSourceOnZ()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 0); 
}

bool BCACC316G::isAsleep()
{ 
  return getRegisterBit(BCACC316G_ACT_TAP_STATUS, 3); 
}

bool BCACC316G::isLowPower()
{ 
  return getRegisterBit(BCACC316G_BW_RATE, 4); 
}

void BCACC316G::setLowPower(bool state) 
{ 
  setRegisterBit(BCACC316G_BW_RATE, 4, state); 
}

double BCACC316G::getRate()
{
  byte _b;
  readFrom(BCACC316G_BW_RATE, 1, &_b);
  _b &= B00001111;
  return (pow(2,((int) _b)-6)) * 6.25;
}

void BCACC316G::setRate(double rate)
{
  byte _b,_s;
  int v = (int) (rate / 6.25);
  int r = 0;
  while (v >>= 1)
  {
    r++;
  }
  if (r <= 9) 
  { 
    readFrom(BCACC316G_BW_RATE, 1, &_b);
    _s = (byte) (r + 6) | (_b & B11110000);
    writeTo(BCACC316G_BW_RATE, _s);
  }
}

void BCACC316G::set_bw(byte bw_code)
{
  if((bw_code < BCACC316G_BW_3) || (bw_code > BCACC316G_BW_1600))
  {
    status = false;
    error_code = BCACC316G_BAD_ARG;
  }
  else
  {
    writeTo(BCACC316G_BW_RATE, bw_code);
  }
}

byte BCACC316G::get_bw_code()
{
  byte bw_code;
  readFrom(BCACC316G_BW_RATE, 1, &bw_code);
  return bw_code;
}


//Used to check if action was triggered in interrupts
//Example triggered(interrupts, BCACC316G_SINGLE_TAP);
bool BCACC316G::triggered(byte interrupts, int mask)
{
  return ((interrupts >> mask) & 1);
}

byte BCACC316G::getInterruptSource() 
{
  byte _b;
  readFrom(BCACC316G_INT_SOURCE, 1, &_b);
  return _b;
}

bool BCACC316G::getInterruptSource(byte interruptBit) 
{
  return getRegisterBit(BCACC316G_INT_SOURCE,interruptBit);
}

bool BCACC316G::getInterruptMapping(byte interruptBit) 
{
  return getRegisterBit(BCACC316G_INT_MAP,interruptBit);
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(BCACC316G_INT_DOUBLE_TAP_BIT,BCACC316G_INT2_PIN);
void BCACC316G::setInterruptMapping(byte interruptBit, bool interruptPin) 
{
  setRegisterBit(BCACC316G_INT_MAP, interruptBit, interruptPin);
}

bool BCACC316G::isInterruptEnabled(byte interruptBit) 
{
  return getRegisterBit(BCACC316G_INT_ENABLE,interruptBit);
}

void BCACC316G::setInterrupt(byte interruptBit, bool state) 
{
  setRegisterBit(BCACC316G_INT_ENABLE, interruptBit, state);
}

void BCACC316G::setRegisterBit(byte regAdress, int bitPos, bool state)  
{
  byte _b;
  readFrom(regAdress, 1, &_b);
  if (state) 
  {
    _b |= (1 << bitPos); // forces nth bit of _b to be 1. all other bits left alone.
  } 
  else 
  {
    _b &= ~(1 << bitPos); // forces nth bit of _b to be 0. all other bits left alone.
  }
  writeTo(regAdress, _b); 
}

bool BCACC316G::getRegisterBit(byte regAdress, int bitPos) 
{
  byte _b;
  readFrom(regAdress, 1, &_b);
  return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void BCACC316G::printAllRegister() 
{
  byte _b;
  Serial.print("0x00: ");
  readFrom(0x00, 1, &_b);
  print_byte(_b);
  Serial.println("");
  int i;
  for (i=29;i<=57;i++)
  {
    Serial.print("0x");
    Serial.print(i, HEX);
    Serial.print(": ");
    readFrom(i, 1, &_b);
    print_byte(_b);
    Serial.println(""); 
  }
}

void print_byte(byte val)
{
  int i;
  Serial.print("B");
  for(i=7; i>=0; i--)
  {
    Serial.print(val >> i & 1, BIN);
  }
}

