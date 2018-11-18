/******************************************************************************
  BasicReadings.ino

  Marshall Taylor @ SparkFun Electronics
  Nathan Seidle @ SparkFun Electronics

  April 4, 2017

  https://github.com/sparkfun/CCS811_Air_Quality_Breakout
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Read the TVOC and CO2 values from the SparkFun CSS811 breakout board

  This is the simplest example.  It throws away most error information and
  runs at the default 1 sample per second.

  A new sensor requires at 48-burn in. Once burned in a sensor requires
  20 minutes of run in before readings are considered good.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5

  Resources:
  Uses Wire.h for i2c operation

  Development environment specifics:
  Arduino IDE 1.8.1

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/
#include "SparkFunCCS811.h"
#include "ArduinoJson.h"
#include "stdint.h"


#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address
CCS811 mySensor(CCS811_ADDR);

//arduino json
const int capacity = JSON_OBJECT_SIZE(3);
StaticJsonBuffer<capacity> jb;

void setup()
{
  Serial.begin(9600);
  Serial.println("CCS811 Basic Example");

  //It is recommended to check return status on .begin(), but it is not
  //required.
  CCS811Core::status returnCode = mySensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error.");
    while (1); //Hang if there was a problem.
  }
}

void loop()
{
  //from the serial channel or terminal send the character 'j'
  if (Serial.read() == 'j') {
    DynamicJsonBuffer jBuffer;
    JsonObject& root = jBuffer.createObject();
    
    //Check to see if data is ready with .dataAvailable()
    if (mySensor.dataAvailable())
    {
      //read the sensor and build the 
      mySensor.readAlgorithmResults();
      root["time"] = 1230;
      root["co2"] = mySensor.getCO2();
      root["tvoc"] = mySensor.getTVOC();

      root.prettyPrintTo(Serial);
      Serial.println();
    }
     
  }
  

  delay(10); //Don't spam the I2C bus
}