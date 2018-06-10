#include <Wire.h>
#include "MPU6050.h"
#include "HX711.h"

MPU6050 mpu;
HX711 scale;

void setup() {
  Serial.begin(115200);

  mpu6050Setup();
  //hx711Setup();
}

void mpu6050Setup() {
   // Initialize MPU6050
  Serial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
  
  // Check settings
  checkSettings();
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:      ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());
  
  Serial.println();
}

void hx711Setup(){
  scale.begin(A0, A1);
  Serial.println("Calibrating power meter");

  scale.set_scale(2280.f);     // this value is obtained by calibrating the scale with known weights
  scale.tare();               // reset the scale to 0
  Serial.println("Power meter calibrated");
}

void loop() {
  // put your main code here, to run repeatedly:
  double cadence;
  double strain;

  cadence = getCadence();
 // strain = scale.get_value(10);
  
  Serial.print(cadence);
  //Serial.print(" ");
 // Serial.print(strain);
  Serial.println("");
  delay(10);
}

/* Returns a new cadence measurement. The cadence is calculated using an 
 * exponential average to weight the previous reading heavily, to reduce
 * the effect of instantaneous spikes in gyroscope readings.
  */
double getCadence(){
  static double avgCadence = 0;
  double newCadence;
  double weight = 0.95;
  Vector normGyro = mpu.readNormalizeGyro();

  // convert the normalized gyro's Z axis reading from deg/s to rpm
  newCadence = ( normGyro.ZAxis ) / (6);

  // apply an exponential average to the cadence reading to smooth out the data
  avgCadence = avgCadence*weight + newCadence*(1 - weight);
  return avgCadence;  
}

