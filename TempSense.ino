/**************************************************************************/
/*!
Derived from the demo of Adafruit MCP9808 breakout
----> http://www.adafruit.com/products/1782

*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"

uint8_t add0 = 0x18;
uint8_t add1 = 0x19;
uint8_t add2 = 0x1A;
uint8_t add3 = 0x1C;
uint8_t add4= 0x1B;
uint8_t add5 = 0x1D;

int delayTime = 1000; //Delay between measurements (ms)

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor0 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor1 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor2 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor3 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor4 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor5 = Adafruit_MCP9808();


void setup() {
  Serial.begin(9600);
  
  // Make sure the sensor is found. Comment out if any sensors are disconnected.
  if (!tempsensor0.begin(add0)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add0, HEX);
    while (1);
  }
    if (!tempsensor1.begin(add1)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add1, HEX);
    while (1);
  }
    if (!tempsensor2.begin(add2)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add2, HEX);
    while (1);
  }
    if (!tempsensor3.begin(add3)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add3, HEX);
    while (1);
  }
    if (!tempsensor4.begin(add4)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add4, HEX);
    while (1);
  }
    if (!tempsensor5.begin(add5)) {
    Serial.print("Couldn't find MCP9808 @ 0x");Serial.println(add5, HEX);
    while (1);
  }
}

void loop() {

  Serial.print("0x"); Serial.print(add0, HEX); Serial.print(": "); Serial.print(tempsensor0.readTempC()); Serial.print("*C\t"); 
  Serial.print("0x"); Serial.print(add1,HEX); Serial.print(": "); Serial.print(tempsensor1.readTempC()); Serial.print("*C\t");
  Serial.print("0x"); Serial.print(add2,HEX); Serial.print(": "); Serial.print(tempsensor2.readTempC()); Serial.print("*C\t");
  Serial.print("0x"); Serial.print(add3,HEX); Serial.print(": "); Serial.print(tempsensor3.readTempC()); Serial.print("*C\t");
  Serial.print("0x"); Serial.print(add4,HEX); Serial.print(": "); Serial.print(tempsensor4.readTempC()); Serial.print("*C\t");
  Serial.print("0x"); Serial.print(add5,HEX); Serial.print(": "); Serial.print(tempsensor5.readTempC()); Serial.print("*C\t");
  Serial.println();
   
  
  delay(1000);
  

  
   
}
