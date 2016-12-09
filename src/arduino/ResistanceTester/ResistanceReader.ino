const float V_IN = 5.0;

double r1 = 10000.0;

double unknownR = 0.0;
float ratio = 0.0;

const float desiredRatio = 0.5;

const int RELAY_ON = LOW, RELAY_OFF = HIGH;

//guesses
float bestRatioDiff = 1.0;
double bestResistanceGuess = 0.0;

// Main methods-------------------------------------------------------------------------------

void printBestResistance() {
  digitalWrite(RELAY3, RELAY_ON);
  delay(50);

  setStartingValues();

  //testing all resistors
  for (int i = 0; i < 3; i++) {
    setRelays(i);
    setR1(i);
    readResistance();

    //find the difference between the current ratio and the desired ratio (where both resistors are the same)
    float ratioDiff = abs(ratio - desiredRatio);

    //set the best result so far
    if (ratioDiff < bestRatioDiff) {
      bestRatioDiff = ratioDiff;
      bestResistanceGuess = unknownR;
    }
    Serial.println();
  }

  //printing results
  if (bestRatioDiff < 1.0)
    printResults();

  digitalWrite(RELAY3, RELAY_OFF);
}

void readResistance() {
  int raw = analogRead(analogPin);

  if (raw)
  {
    float scaled = raw * V_IN;
    float vout = (scaled) / 1024.0;
    ratio = vout/V_IN;  
    unknownR = r1 * (V_IN - vout) / vout; // V = IR, R2 = R1*(Vin - Vout)/Vout
  }
}

// Helper methods------------------------------------------------------------------------------

/**
   There are 3 resistors that the relays can be programmed to test with, numbered 0, 1, 2.
   Given the number of a resistor, this function turns on/off the proper relays.
*/
void setRelays(int whichResistor) {
  switch (whichResistor) {
    case 0:
      digitalWrite(RELAY1, RELAY_ON);
      break;
    case 1:
      digitalWrite(RELAY1, RELAY_OFF);
      digitalWrite(RELAY2, RELAY_OFF);
      break;
    case 2:
      digitalWrite(RELAY1, RELAY_OFF);
      digitalWrite(RELAY2, RELAY_ON);
      break;
  }
  delay(100);
}

void setR1(int whichResistor) {
  switch (whichResistor) {
    case 0:
      r1 = 97.6;
      break;
    case 1:
      r1 = 10e3;
      break;
    case 2:
      r1 = 931e3;
      break;
  }
}

void printResults() {
  Serial.print("R2: ");
  Serial.println(bestResistanceGuess);

  if (USE_LCD) {
    lcd.setCursor(0, 0); //Start at character 0 on line 0
    lcd.print("The resistance is:");
    lcd.setCursor(0, 1);
    lcd.print(String(bestResistanceGuess));
  }
}

void setStartingValues() {
  bestRatioDiff = 1.0;
  unknownR = 0.0;
  ratio = 0.0;
}

void writeDigiPotRes(byte val) {
  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmittings
  r1 = 60 + (val * 39);         // potentiometer resistance RWB = RAB/256 + RW = 39*val + 60
}



