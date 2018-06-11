#include "HX711.h"

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  scale.begin(A0, A1);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(scale.averageValue());
  delay(200);
}
