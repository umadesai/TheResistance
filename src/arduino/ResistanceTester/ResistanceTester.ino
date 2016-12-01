/*
   Example voltage divider resistance measurer
   Code from http://www.circuitbasics.com/arduino-ohm-meter/
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>

int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 10000;
float R2 = 0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

Stepper motor(64, 2, 3, 4, 5);

boolean testResistance = true;
int numFastSteps = 342; //total of 2055, divided by 2, roounded down
int numSlowSteps = 343; //total of 2055, divided by 2, rounded up

void setup()
{
  Serial.begin(9600);

  //lcd
  //  lcd.begin(20, 4);
  //  lcd.backlight();
  //  lcd.setCursor(0, 0); //Start at character 0 on line 0
  //  lcd.print("The Resistance");
  //  delay(1000);

  //stepper
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  motor.setSpeed(200);
}

void loop()
{
  if (testResistance) {
    int stepSize = 1;
    for (int i = 0; i <= numSlowSteps; i += stepSize) {
      //since the stepper is blocking, we want to step a step at a time.
      motor.step(stepSize);
      delay(10);
      readResistance();
      delay(10);
    }
  }
  else {
    Serial.println("FAST\n\n\n\n");
    motor.step(numFastSteps);
  }
  delay(100);
  testResistance = !testResistance;
}



void readResistance() {
  //replace below code with code to test resistance
  raw = analogRead(analogPin);

  if (raw)
  {
    float scaled = raw * Vin;
    Vout = (scaled) / 1024.0;
    float ratio = (Vin / Vout) - 1;
    R2 = R1 * ratio;

    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);

    //    lcd.setCursor(0, 0); //Start at character 0 on line 0
    //    lcd.print("The resistance is:");
    //    lcd.setCursor(0, 1);
    //    lcd.print(String(R2));
  }
}

