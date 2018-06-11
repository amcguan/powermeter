#include <Wire.h>
#include "MPU6050.h"
#include "HX711.h"

MPU6050 mpu;
HX711 scale;

void setup() {
  Serial.begin(115200);

  //mpu6050Setup();
  //hx711Setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  double cadence;
  double strain;
  double normalAvgVelocity, circularVelocity;

//  normalAvgVelocity = getNormalAvgVelocity(normalAvgVelocity);
//  circularVelocity = getCircularVelocity(normalAvgVelocity);
//  cadence = getCadence(normalAvgVelocity);
  
  cadence = 10;
  strain = 14.56;
 // printDisplay(cadence, strain);
  Serial.write('c');
  Serial.println(cadence);
  Serial.write('s');
  Serial.println(strain);
  delay(940);
}




