/*
   Example voltage divider resistance measurer
   Code from http://www.circuitbasics.com/arduino-ohm-meter/
*/

#include <LiquidCrystal_I2C.h> //requires latest LiquidCrystal library
#include <Wire.h>

int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup()
{
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("The Resistance");
  delay(1000);
}

void loop()
{
  raw = analogRead(analogPin);
  if (raw)
  {
    buffer = raw * Vin;
    Vout = (buffer) / 1024.0;
    buffer = (Vin / Vout) - 1;
    R2 = R1 * buffer;
    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);
    lcd.setCursor(0, 0); //Start at character 4 on line 0
    lcd.print("Resistance: " + String(R2));
    delay(1000);
  }
}
