#include <Wire.h>
int analogPinWiper = A1;
int analogPinVout = A0;
int Vin = 1023;
float Vout = 0;
float unknownR = 0;
float knownR = 0;
float voltageRatio = 0;
float wiperValue = 0;
float difference; 
int state = 0;
int count = 0;
int lowestVal = 0;
float lowestDiff = 0;
float diffVout = 0;
float lowDiffVout = 0;
int nextRelay = false;

const int Relay1 = 11;
const int Relay2 = 12;
const int Relay3 = 13;


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
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3, OUTPUT);
}

byte val = 0;

void loop()
{
   Re1 = LOW; //turn on power to relays
      digitalWrite(Relay1, Re1);
      digitalWrite(Relay2, Re2);
      digitalWrite(Relay3, Re3);
      
  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmittings

  Vout = analogRead(analogPinVout); //read voltage in between resistors
  knownR = 60 + (val*39);           // potentiometer resistance RWB = RAB/256 + RW = 39*val + 60
  if (Vout)
  {
    voltageRatio = Vout / (Vin - Vout);    // V = IR, R2 = R1*Vout/(Vin - Vout) 
    unknownR = voltageRatio * knownR;      
  }

  Serial.print("Val: ");
  Serial.print(val);
  Serial.print(",  Vout:");
  Serial.print(Vout);
  Serial.print(",  state:");
  Serial.print(state);
  Serial.print(",  Difference:");
  Serial.print(difference);
  Serial.print(",  LowestVal:");
  Serial.print(lowestVal);
  Serial.print(",  LowestDiff:");
  Serial.print(lowestDiff);
  Serial.print(",  knownR:");
  Serial.print(knownR);
  Serial.print(",  unknownR:");
  Serial.println(unknownR);
  

  difference = pow((unknownR - knownR),2);
  diffVout = pow(((1023/2) - Vout),2);

  if ((state == 0) && (val == 20)){
    lowestVal = val;
    lowestDiff = difference;
    lowDiffVout = diffVout;
  }
  else{
    if ((difference < lowestDiff) && (diffVout <= lowDiffVout)){
      lowestVal = val;
      lowestDiff = difference;
      lowDiffVout = diffVout;

    }
  }

  if (state == 0){
  val+=10;           // increment value by 10s
  if (val > 250) { // if it passes 256th position (max), reset to zero
    val = lowestVal - 10;
    state = 1;
  }                 // start over from lowest value
  }


  if (state == 1){
  val+=1;           // increment value by 10s
  if (val >= lowestVal + 10) { // if it passes 256th position (max), reset to zero
    state = 2;
  }                 // start over from lowest value
  }

  if (state == 2){
    if (lowDiffVout >= 10000){ //if the lowest vout difference is higher than 100^2, we should try again at the next relay
      nextRelay = true;
    }
    Serial.print("END: unknown R is ");
    Serial.print(unknownR);
     delay(100000);
    return lowestVal, lowestDiff;
    delay(100000);
    
  }
  
    
  
 
  delay(1000);
  
}

