
// Include the AccelStepper library:
#include "AccelStepper.h"


// Include stepper motor functions

// Define stepper motor connections and motor interface type.  
// Motor interface type must be set to 1 when using a driver
#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 200


void setup() {

  Serial.begin(9600);

  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(10,INPUT);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);


  
  }

  

void loop(){


  Serial.print(digitalRead(10));

  /*

  for(int i = 0; i < 5*stepsPerRevolution; i++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    
  }

  delay(1000);
  
  
  stepper.setCurrentPosition(0);

  while( stepper.currentPosition() != 400){
    stepper.setSpeed(3200);
    stepper.runSpeed();
  }


  delay(200);

  stepper.setCurrentPosition(0);

  while( stepper.currentPosition() != -100){
    stepper.setSpeed(-700);
    stepper.runSpeed();
  }

  

  delay(800);

  */
}
