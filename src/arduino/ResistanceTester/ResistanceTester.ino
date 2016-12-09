/*
   The Resistance
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <Stepper.h>

const int analogPin = 0, vinPin = 1;
const int RELAY1 = 11, RELAY2 = 12, RELAY3 = 13;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//stepper in
int IN1 = 2, IN2 = 3, IN3 = 4, IN4 = 5;
int currentStep = 0; //stepper control

const boolean USE_LCD = true;

void setup()
{
  Serial.begin(9600);

  //lcd
  if (USE_LCD) {
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 0); //Start at character 0 on line 0
    lcd.print("The Resistance");
    delay(1000);
  }

  //stepper
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
}

void loop()
{
  //since the stepper is blocking, we want to step a step at a time.
  for (int i = 0; i < 20; i++) {
    singleStep();
    delay(10);
  }
//  delay(500);
  printBestResistance();
}


void singleStep() {
  switch (currentStep) {
    case 0:
      writeStepperPins(LOW, LOW, LOW, HIGH);
      break;
    case 1:
      writeStepperPins(LOW, LOW, HIGH, HIGH);
      break;
    case 2:
      writeStepperPins(LOW, LOW, HIGH, LOW);
      break;
    case 3:
      writeStepperPins(LOW, HIGH, HIGH, LOW);
      break;
    case 4:
      writeStepperPins(LOW, HIGH, LOW, LOW);
      break;
    case 5:
      writeStepperPins(HIGH, HIGH, LOW, LOW);
      break;
    case 6:
      writeStepperPins(HIGH, LOW, LOW, LOW);
      break;
    case 7:
      writeStepperPins(HIGH, LOW, LOW, HIGH);
      break;
    default:
      writeStepperPins(LOW, LOW, LOW, LOW);
      break;
  }
  currentStep -= 2;
  if (currentStep < 0)
    currentStep = 7;
}

void writeStepperPins(int in1Val, int in2Val, int in3Val, int in4Val) {
  digitalWrite(IN1, in1Val);
  digitalWrite(IN2, in2Val);
  digitalWrite(IN3, in3Val);
  digitalWrite(IN4, in4Val);
}

