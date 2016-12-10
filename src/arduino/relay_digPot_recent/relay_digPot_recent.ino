#include <Wire.h>
int analogPinVin = A1;
int analogPinVout = A0;
int Vin = 1023;
float Vout = 0;
float unknownR = 0;
float knownR = 0;
int state = 0; 
int lowestVal = 0; //val at which lowest difference in Vin and Vout appears
float diffVout = 0;
float lowDiffVout = 100000;
bool nextRelay = false;
int relayNum = 0;
float resistances[256];
const int Relay1 = 11;
const int Relay2 = 12;
const int Relay3 = 13;
int rval = 2;


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int Re1 = HIGH;
int Re2 = HIGH;
int Re3 = HIGH;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
}

byte val = 4;

void loop()
{
  if (relayNum == 0) {
    Re1 = LOW; //turn on power to relays
    Re2 = HIGH;
    Re3 = HIGH;
    relayNum = 1;
  }
  digitalWrite(Relay1, Re1);
  digitalWrite(Relay2, Re2);
  digitalWrite(Relay3, Re3);
  Wire.beginTransmission(46); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmittings
  Vout = analogRead(analogPinVout); //read voltage in between resistors
  Vin = analogRead(analogPinVin);
  
  if (rval == 1){ //10k po
  knownR = 30.524 + ((float)val * 38.34);         // potentiometer resistance RWB = RAB/256 + RW = 39*val + 60
  }
  else if (rval == 2){ //100k pot
  knownR = 174.17 + ((float)val * 384.62);         // potentiometer resistance RWB = RAB/256 + RW = 39*val + 60
  }

  else if (rval == 3){ //1M pot
  knownR = (-0.1644 * (float)(pow(val,3))) + (64.274 * (float)(pow(val,2))) - (1652.7 * (float) val) + 153635. ;         // potentiometer resistance RWB = RAB/256 + RW = 39*val + 60
  }
  
  
  if (Vout)
  {
    //voltageRatio = (Vin - Vout)/Vin;    // V = IR, R2 = R1*Vout/(Vin - Vout)
   // unknownR = voltageRatio * knownR;
   //unknownR = ((knownR*(float)Vin)/((float)Vout)) - knownR;
      unknownR = knownR;
  }
  diffVout = pow(((Vin/2) - Vout), 2);
  resistances[val] = knownR;


  Serial.print("Val: ");
  Serial.print(val);
  Serial.print(",  Vout:");
  Serial.print(Vout);
  Serial.print(",  state:");
  Serial.print(state);
  Serial.print(",  LowestVoutDiff:");
  Serial.print(lowDiffVout);
  Serial.print(",  LowestVal:");
  Serial.print(lowestVal);
  Serial.print(",  knownR:");
  Serial.print(knownR);
  Serial.print(",  unknownR:");
  Serial.println(unknownR);

      //delay(20000);


  if ((diffVout < lowDiffVout)) {
    lowestVal = val;
    lowDiffVout = diffVout;
  }

  if (state == 0) {
    val += 20;         // increment value by 10s
    if (val > 240) { // if it passes 256th position (max), reset to zero
      val = lowestVal - 20;
      state = 1;
    }                 // start over from lowest value
  }


  else if (state == 1) {
    val += 10;         // increment value by 10s
    if (val >= (lowestVal + 20)) { // if it passes 256th position (max), reset to zero
      state = 2;
      val = lowestVal - 10;
    }                 // start over from lowest value
      if (val >= (250)) { // if it passes 256th position (max), reset to zero
      state = 2;
      val = lowestVal - 10;
    }   
  }

    else if (state == 2) {
    val += 1;         // increment value by 10s
    if (val >= (lowestVal + 10)) { // if it passes 256th position (max), reset to zero
      state = 3;
    }                 // start over from lowest value
  }

  else if (state == 3) {
    if (lowDiffVout >= 10000) { //if the lowest vout difference is higher than 100^2, we should try again at the next relay
      nextRelay = true;
    }
    Serial.print("END: unknown R is ");
    Serial.print(resistances[lowestVal]);
    Re1 = HIGH;
    state = 3;
    delay(20000);

  }

  if (nextRelay == true) {
    if (relayNum == 1) {
      Re1 = LOW;
      Re2 = LOW;
      rval = 1;
      relayNum = 2;
    }
    else if (relayNum == 2) {
      Re1 = LOW;
      Re2 = LOW;
      Re3 = LOW;
      relayNum = 3;
    }
    else {
      relayNum = 0;
      Serial.print("ERROR: RESTARTING");
    }
    nextRelay = false; //set nextRelay to false since we have moved to the next relay
    state = 0;         //reset state to zero
  }



  //delay(5000);

}

