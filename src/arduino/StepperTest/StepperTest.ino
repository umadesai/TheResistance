#include <Stepper.h>

Stepper motor(64, 2, 3, 4, 5);

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  motor.setSpeed(300);
  Serial.begin(9600);
}

int numLoops = 0;

void loop() {
  //using serial to control when the stepper starts and stops
  while(Serial.read() == -1){
   //step until the user sends something over serial
   motor.step(1);
   delay(10);
  }
  
  while(Serial.read() != -1); //clear buffer
  
  while(Serial.read() == -1){
   //do nothing until user sends something over serial
   delay(10);
  }

  while(Serial.read() != -1); //clear buffer
}
