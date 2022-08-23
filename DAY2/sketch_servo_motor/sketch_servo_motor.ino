
#include <Servo.h>

Servo myservo1;

//motor control mode pin
#define ENA 13
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
#define ENB 8


//servo motor PWM pin
#define servo1 5

void stop_motor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void setup() {
//  pinMode(servo1, OUTPUT);
//  myservo1.attach(servo1);
//  myservo1.write(135);
//  delay(1000);
}

void collect_redball(){
//  stop_motor();
//  myservo1.write(0);
//  delay(1000);
//  myservo1.write(135);
//  delay(1000);
}
