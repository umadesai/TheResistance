float r1 = 10000.0;
int vin = 1023;

double unknownR = 0.0;
float ratio = 0.0;

const float desiredRatio = 0.5;

const int RELAY_ON = LOW, RELAY_OFF = HIGH;

const float SUCCESS_THRESHOLD = 0.05, WAY_OFF_THRESHOLD = 0.3; /*CHANGE THESE THRESHOLDS IF NECESSARY*/

//guesses
float bestRatioDiff = 1.0;
double bestResistanceGuess = 0.0;

// Main methods-------------------------------------------------------------------------------

float bestResistance() {
  digitalWrite(RELAY3, RELAY_ON);
  delay(50);

  setStartingValues();

  //testing all 3 resistors/digital potentiometers
  for (int i = 0; i < 3; i++) {
    setRelays(i);
    int digiPotVal = 0; //store it as an int, so we can chack if it goes over. Cast it as a byte before we send it.

    while (digiPotVal < 256) {
      writeDigiPotResistance((byte)digiPotVal, i);
      readResistance();

      //find the difference between the current ratio and the desired ratio (where both resistors are the same)
      float ratioDiff = abs(ratio - desiredRatio);

      //set the best result so far
      if (ratioDiff < bestRatioDiff) {
        bestRatioDiff = ratioDiff;
        bestResistanceGuess = unknownR;
      }

//      //set the next value of digiPotVal based on how far we are from the ideal
//      if (ratioDiff > WAY_OFF_THRESHOLD) {
//        digiPotVal += 20;
//      }
//      else if (ratioDiff < SUCCESS_THRESHOLD) {
//        //we found a really great value, quit looking for better values
//        i = 4;
//        break;
//      }
//      else {
//        digiPotVal++;
//      }
    }
  }

  //printing results
  if (bestRatioDiff < 1.0)
    printResults();

  digitalWrite(RELAY3, RELAY_OFF);
  return bestResistanceGuess;
}

void readResistance() {
  int vout = analogRead(analogPin);
  vin = analogRead(vinPin);
  if (vout)
  {
    ratio = ((float)vout) / vin;
    unknownR = r1 * (vin - vout) / vout; // V = IR, R2 = R1*(Vin - Vout)/Vout
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
    lcd.clear();
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

void writeDigiPotResistance(byte val, int whichResistor) {
  int address = 0;
  if (whichResistor == 0) {
    //10k
    address = 44;
    r1 = 30.524 + ((float)val * 38.34);
  }
  else if (whichResistor == 1) {
    //100K
    address = 46;
    r1 = 174.17 + ((float)val * 384.62);
  }
  else if (whichResistor == 2) {
    //1M
    address = 45;
    r1 = (-0.1644 * (float)(pow(val, 3))) + (64.274 * (float)(pow(val, 2))) - (1652.7 * (float) val) + 153635;
  }
  Wire.beginTransmission(address);
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmittings
}




