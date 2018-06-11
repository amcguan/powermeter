
#include "HX711.h"

#define HX711_OFFSET 2280.f

void hx711Setup(){
  scale.begin(A0, A1);
  Serial.println("Calibrating power meter");

  scale.set_scale(HX711_OFFSET);     // this value is obtained by calibrating the scale with known weights
  scale.tare();               // reset the scale to 0
  Serial.println("Power meter calibrated");
}

