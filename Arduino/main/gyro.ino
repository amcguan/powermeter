
#define WEIGHT 0.95
#define CRANK_R 0.175

// ---------------------------------------------------------------------------
// ====================== SETUP CODE =========================================
// ---------------------------------------------------------------------------
/** 
 *  Calibrate and initialize the gyroscope
 */
void mpu6050Setup() {
   // Initialize MPU6050
  Serial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  // Calibrate gyroscope. The calibration must be at rest.
  mpu.calibrateGyro();
  // Set threshold sensivty. Default 3.
  mpu.setThreshold(3);
  // Check settings
  checkSettings();
}

void checkSettings(){
  Serial.println();
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  Serial.print(" * Clock Source:      ");
  switch(mpu.getClockSource()){
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale()){
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

// ---------------------------------------------------------------------------
// ====================== LOOP CODE =========================================
// ---------------------------------------------------------------------------

/**
 * Gets a nromalized averaged rotational velocity calculation. The MPU6050 library supports a 
 * normalized gyroscope reading, which trims off outliers and scales the values to deg/s. An
 * exponential average is applied to further smooth data, with weight of WEIGHT. 
 */
double getNormalAvgVelocity(double last){
  double oldAvg = last;
  double newData;
  Vector normGyro = mpu.readNormalizeGyro();

  newData = (normGyro.ZAxis);
  return oldAvg * (WEIGHT) + newData *(1-WEIGHT);
}

/**
 * Returns the circular velocity of the rider's foot. Takes in the crank's averaged rotational
 * velocity, converts it to radians, multiplies by the crank radius, and returns the converted 
 * value.
 */
double getCircularVelocity(double normAvgRotate){
  return (normAvgRotate * 2 * PI * CRANK_R) / 360;
}

/** Returns a new cadence measurement. Cadence is the normalized angular velocity,
 *  times 60/360, which converts from deg/s to rotations/min.
 *  In other words, divide normalized rotational velocity by 6.
 */
double getCadence(double normAvgRotate){
  return normAvgRotate / 6;
}
