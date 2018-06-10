#include "HX711.h"

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  scale.begin(A0, A1);
  Serial.println("Taring scale");
  scale.set_scale();
  scale.tare();
  Serial.println("Scale setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  double value;
  Serial.println("You have 5 seconds to place a weight on the meter");
  delay(5000);

  value = scale.get_units(10);
  Serial.print("Read value: ");
  Serial.println(value);
}
