#include <Servo.h>

Servo myservo1;
////motor control mode pin
#define ENA 8
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12
#define ENB 13
//wheel speed
#define speedleft 50
#define speedright 66
#define servo1 5
//supersonic mode pin
#define R_trigger 30
#define R_echo 31
#define C_trigger 26
#define C_echo 27

int L_cm = 0, R_cm = 0, C_cm = 0;
long R_duration, L_duration, C_duration;
const int turn_Distance = 20;
bool startup = 0;
void stop_motor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void servo_setup() {
  pinMode(servo1, OUTPUT);
  myservo1.attach(servo1);
  myservo1.write(135);
  delay(1000);
}

int go_straight(int left,int right){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, left);
  analogWrite(ENB, right);
}
//void go_rightfornt(){
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, HIGH);
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//  analogWrite(ENA, speedleft);
//  analogWrite(ENB, speedright-15);
//}

void go_back(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedleft);
  analogWrite(ENB, speedright);
}

void turn_right(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedleft);
  analogWrite(ENB, speedright);
}

void turn_left(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedleft);
  analogWrite(ENB, speedright);
}
void Distance(){

  digitalWrite(R_trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(R_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(R_trigger, LOW);

  pinMode(R_echo, INPUT);
  R_duration = pulseIn(R_echo, HIGH);
 
  R_cm = (R_duration/2) / 29.1;

  digitalWrite(C_trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(C_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(C_trigger, LOW);

  pinMode(C_echo, INPUT);
  C_duration = pulseIn(C_echo, HIGH);
 
  C_cm = (C_duration/2) / 29.1;
}
void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(R_trigger, OUTPUT);
  pinMode(C_trigger, OUTPUT);  
  pinMode(R_echo, INPUT);
  pinMode(C_echo, INPUT);
  
  delay(500);
}
void loop() {
  //servo_setup();
  Distance();
  Serial.print("C:");
  Serial.print(C_cm);
  Serial.print("R:");
  Serial.print(R_cm);
  Serial.print("\n");

  if(C_cm < turn_Distance-5){
    go_back();
    delay(1000);
    turn_left();
    delay(800);
    stop_motor();
    delay(1000);
    startup =1;
  }
//  if(R_cm < turn_Distance-1){
//    go_straight(speedleft-20, speedright);
////    delay(50);
//  }
//  if(R_cm > turn_Distance+1){
//    go_straight(speedleft, speedright-20);
////    delay(50);
////    stop_motor();
//  }
//  if((R_cm <= turn_Distance +3)&&(R_cm >= turn_Distance -3)){
  if(startup == 1){
    go_straight(30,0);
    delay(200);
  }
    go_straight(speedleft, speedright);
    startup=0;
}
  
