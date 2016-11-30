#include <Wire.h>
int analogPinWiper = A1;
int analogPinVout = A0;
int Vin = 1023;
float Vout = 0;
float unknownR = 0;
float knownR = 0;
float voltageRatio = 0;
float wiperValue = 0;
int difference[256]; 
int state = 0;
int count = 0;
int lowestVal = 0;
int lowestDiff = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

byte val = 0;

void loop()
{
  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
  // device address is specified in datasheet
  Wire.write(byte(0x00));            // sends instruction byte
  Wire.write(val);             // sends potentiometer value byte
  Wire.endTransmission();     // stop transmitting

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
  Serial.print(",  Difference:");
  Serial.print(difference[val]);
  Serial.print(",  LowestVal:");
  Serial.print(lowestVal);
  Serial.print(",  LowestDiff:");
  Serial.print(lowestDiff);
  Serial.print(",  knownR:");
  Serial.print(knownR);
  Serial.print(",  unknownR:");
  Serial.println(unknownR);
  

  difference[val] = abs(unknownR - knownR);

  if (val == 20){
    lowestVal = val;
    lowestDiff = difference[val];
  }
  else{
    if (difference[val] < lowestDiff){
      lowestVal = val;
      lowestDiff = difference[val];
    }
  }

  if (state == 0){
  val+=10;           // increment value by 10s
  if (val >= 256) { // if it passes 256th position (max), reset to zero
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
    return lowestVal, lowestDiff;
  }
  
    
  
 
  delay(1000);
  
}

