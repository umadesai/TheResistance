const float V_IN = 5.0;
//need to add more resistances and have a variable equal to the current known resistance (based on the relays)
double R1 = 10000.0;

double unknownR = 0.0;
float ratio = 0.0;

const float desiredRatio = 0.5;

float bestRatioDiff = 1.0;
double bestResistanceGuess = 0.0;

void printBestResistance() {
  digitalWrite(RELAY3, HIGH);
  delay(10);

  //testing all resistors
  for (int i = 0; i < 3; i++) {
    setRelays(i);
    readResistance();

    //find the difference between the current ratio and the desired ratio (where both resistors are the same)
    float ratioDiff = abs(ratio - desiredRatio);

    //set the best result so far
    if (ratioDiff < bestRatioDiff) {
      bestRatioDiff = ratioDiff;
      bestResistanceGuess = unknownR;
    }
  }

  //printing results
  Serial.print("R2: ");
  Serial.println(bestResistanceGuess);

  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("The resistance is:");
  lcd.setCursor(0, 1);
  lcd.print(String(bestResistanceGuess));

  //reset
  bestRatioDiff = 1.0;
  unknownR = 0.0;
  ratio = 0.0;

  digitalWrite(RELAY3, LOW);
}

void readResistance() {
  //replace below code with code to test resistance
  int raw = analogRead(analogPin);

  if (raw)
  {
    float scaled = raw * V_IN;
    float Vout = (scaled) / 1024.0;
    //need to change bottom to account that r2 is the known
    ratio = (V_IN / Vout) - 1;
    unknownR = R1 * ratio;
  }
}

/**
   There are 3 resistors that the relays can be programmed to test with, numbered 0, 1, 2.
   Given the number of a resistor, this function turns on/off the proper relays.
*/
void setRelays(int whichResistor) {
  switch (whichResistor) {
    case 0:
      digitalWrite(RELAY1, HIGH);
      break;
    case 1:
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, HIGH);
      break;
    case 2:
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      break;
  }
}

