#include <Stepper.h>

//Stepper motor(64, 2, 3, 4, 5);

int IN1 = 2, IN2 = 3, IN3 = 4, IN4 = 5;
int currentStep = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
//  motor.setSpeed(300);
  Serial.begin(9600);
  Serial.println("Start");
}


void loop() {
  //using serial to control when the stepper starts and stops
  int count = 0;
  while(Serial.read() == -1){
   //step until the user sends something over serial
   singleStep();
   count++;
   delay(10);
  }

  Serial.println(count);
  while(Serial.read() != -1); //clear buffer
  
  while(Serial.read() == -1){
   //do nothing until user sends something over serial
   delay(10);
  }

  while(Serial.read() != -1); //clear buffer
}

void singleStep(){
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
    currentStep-=2;
    if (currentStep < 0)
      currentStep = 7;
}

void writeStepperPins(int in1Val, int in2Val, int in3Val, int in4Val) {
  digitalWrite(IN1, in1Val);
  digitalWrite(IN2, in2Val);
  digitalWrite(IN3, in3Val);
  digitalWrite(IN4, in4Val);
}

