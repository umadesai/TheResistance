/*
   Example voltage divider resistance measurer
   Code from http://www.circuitbasics.com/arduino-ohm-meter/
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

Servo ferrisWheel;

void setup()
{
  Serial.begin(9600);

  //lcd
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("The Resistance");
  delay(1000);

  //servo
  ferrisWheel.attach(9);
}

void loop()
{
  for (int theta = 0; theta <= 180; theta++) {
    turnServo(theta);
  }
  for (int theta = 180; theta >= 0; theta--) {
    turnServo(theta);
  }
}

void turnServo(int theta) {
  ferrisWheel.write(theta);
  if (theta % 90 == 0) {
    for(int i = 0; i<100; i++){
      readResistance();
    }
  }
  else {
    delay(100);
  }

}

void readResistance() {
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

    lcd.setCursor(0, 0); //Start at character 0 on line 0
    lcd.print("The resistance is:");
    lcd.print(String(R2));

    delay(1000);
  }
}

