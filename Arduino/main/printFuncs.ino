#define NUM_LABELS 2
#define DELAY 0

void printDisplay(double cadence, double strain){
  home();
  border();
  printLabels(cadence, strain);
}

void home(){
  Serial.write(0x1B);
  Serial.print("[H");
}

void border(){
  Serial.print("=============================================");
  delay(DELAY);
  Serial.write(0x1B);
  Serial.write("[");
  Serial.write(NUM_LABELS+48+2);
  Serial.print(";1f");
  delay(DELAY);
  Serial.print("=============================================");
}

void printLabels(double cadence, double strain){
  home();
  Serial.write(0x1B);
  Serial.write("[2;5f");
  delay(DELAY);
  Serial.write("cadence:");
  Serial.print(cadence);
  delay(DELAY);
  Serial.write(0x1B);
  Serial.write("[3;5f");
  delay(DELAY);
  Serial.write("strain:");
  Serial.print(strain);
  delay(DELAY);
}


