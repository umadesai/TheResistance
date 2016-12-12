#include <Wire.h>
int analogPinVin = A1; //Read actual voltage coming out of 5V Arduino pin
int analogPinVout = A0; // Read Vout between known and unknown resistors
int Vin = 1023; // Initialize Vin at 1023 which corresponds to 5V

float Vout = 0; // Initialize Vout at 0
float unknownR = 0; // Introduce unknown R as a float ??????????????????? need ???????????????
float knownR = 0; // Introduce known R as a float (this can handle values as large as 3.4028235E+38)

int state = 0;  // initroduce state as an integer (used to keep tab of stepsize when increasing resistance 0-256)
int lowestVal = 0; //val at which lowest difference in Vin and Vout appears
// this val can only be an integer between 0 and 256

float diffVout = 0; // difference between Vin/2 and Vout
float lowDiffVout = 100000; // set low difference in Vout such that it will always be larger than diffVout in the beginning <<<<<<<<<<<<<<<<<<<<<<<<<<<<

bool nextRelay = false; // Relay control starts??????????????????
int relayNum = 0; //

float resistances[256]; //?????????????????????????????????????????????????

//where the relays are connected to digital Arduino output pins
const int Relay1 = 11;  // Relay IN1, labeled Relay 1 in schematic
const int Relay2 = 12; // Relay IN2, labeled Relay 2 in schematic
const int Relay3 = 13; // Relay IN3, labeled Relay 3 in schematic 

int rval = 2; // still important with Relays integrated?????????????????

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

// start all relays at HIGH (connected as originally connected in curciut diagram at the pin connection port of relays) ????????????
int Re1 = HIGH; 
int Re2 = HIGH;
int Re3 = HIGH;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //set relays as outputs
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
}

byte val = 4; // initialize val at 4 (REASON??????)

void loop()
{
  if (relayNum == 0) {
    Re1 = LOW; // turn off Re1
    Re2 = HIGH; // keep Re2 on
    Re3 = HIGH; // keep Re3 on
    relayNum = 1; // set relayNum to 1
  }
  
  digitalWrite(Relay1, Re1); // set Relay1 to Re1 value, etc.
  digitalWrite(Relay2, Re2);
  digitalWrite(Relay3, Re3);
  
  Wire.beginTransmission(46); // transmit to device of given decimal address (0x2c)
  // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmittings
  Vout = analogRead(analogPinVout); //read voltage in between resistors
  Vin = analogRead(analogPinVin); // read voltage at 5V terminal of breadboard

  // Define knownR calculation based on the potentiometer used to obtain value
  if (rval == 1){ //10k pot
  knownR = 30.524 + ((float)val * 38.34);         // potentiometer resistance RWB = RAB/256 + RW = ~39*val + const.
  }
  else if (rval == 2){ //100k pot
  knownR = 174.17 + ((float)val * 384.62);         // potentiometer resistance calculation after calibration
  }

  else if (rval == 3){ //1M pot
  knownR = (-0.1644 * (float)(pow(val,3))) + (64.274 * (float)(pow(val,2))) - (1652.7 * (float) val) + 153635. ;         // potentiometer resistance calculation after calibration
  }
  
  if (Vout) // do we need this if?
  {unknownR = knownR;} // since the Vout is closest to Vin/2 when unknownR = knownR
   
  diffVout = pow(((Vin/2) - Vout), 2); // defines diffVout
  resistances[val] = knownR;

//print values to check accuracy
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

  if ((diffVout < lowDiffVout)) { // if the difference (Vout - Vin/2) is lower than lowest value recorded thus far
    lowestVal = val; // set the current val as the lowestVal (where one can find the lowest difference)
    lowDiffVout = diffVout; // and set the new lowest difference value as such
  }

  if (state == 0) {
    val += 20;         // increment value by 20s
    if (val > 240) { // if it passes 240th position (max while incrementing in 20s)
      val = lowestVal - 20; // go back to val 20 points below where lowest value was recorded
      state = 1; // change to state 1
    } 
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
    val += 1;         // increment value by 1
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

void 
