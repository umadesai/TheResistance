/*
   The Resistance
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <Stepper.h>

const int analogPin = 0, vinPin = 1;
const int RELAY1 = 11, RELAY2 = 12, RELAY3 = 13;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
const boolean USE_LCD = true;

//stepper in
int IN1 = 3, IN2 = 4, IN3 = 5, IN4 = 6;
int currentStep = 0; //stepper control

//button
int BUTTON = 2;

void setup()
{
  Serial.begin(9600);
  Serial.println("The Revolution");
  //stepper
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  //lcd
  if (USE_LCD) {
    lcd.begin(20, 4);
    lcd.backlight();
    delay(100);
    lcd.clear();
    lcd.setCursor(3, 0); //Start at character 4 on line 0
    lcd.print("The Resistance");
    singleStep(); //stepping the stepper motor, because that somehow affects the bg of the lcd
    lcd.setCursor(6, 1);
    lcd.print("presents");
    singleStep(); //the stupidest thing ever
    lcd.setCursor(3, 3);
    lcd.print("The Revolution");
    singleStep();
    delay(5000);
  }

  //calibration
  pinMode(BUTTON, INPUT);


  if (USE_LCD) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press the button");
    lcd.setCursor(0, 1);
    lcd.print("");
    lcd.print("when a notch on the");
    lcd.setCursor(0, 2);
    lcd.print("wheel is aligned");
    lcd.setCursor(0, 3);
    lcd.print("with the ramp");
  }

  boolean initButton = digitalRead(BUTTON);
  while (digitalRead(BUTTON) == initButton) {
    singleStep();
    delay(20);
  }

  delay(250);
  for (int i = 0; i < 343; i++) {
    singleStep();
    delay(10);
  }

   if (USE_LCD) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("When you are ready,");
    lcd.setCursor(0, 1);
    lcd.print("drop a resistor onto");
    lcd.setCursor(6, 2);
    lcd.print("the ramp");
    delay(5000);
  }
}

void loop()
{
  //since the stepper is blocking, we want to step a step at a time.
  for (int i = 0; i < 685; i++) {
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

