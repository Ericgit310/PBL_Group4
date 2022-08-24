#include <Pixy2.h>
#include <Servo.h>
Servo myservo1;
Pixy2 pixy;
////motor control mode pin
#define ENA 8
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12
#define ENB 13
//wheel speed
#define speedleft 150
#define speedright 100
#define speedleft 120
#define speedright 120
#define servo1 5
uint16_t blocks;
int sig[3], x[3], width[3];
int red_block, white_block;
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

void collect_redball(){
  stop_motor();
  myservo1.write(0);
  delay(1000);
  myservo1.write(135);
  delay(1000);
}
void go_straight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedleft);
  analogWrite(ENB, speedright);
}

void go_back(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
void ditect_redball(){
  pixy.ccc.getBlocks();
  blocks = pixy.ccc.numBlocks;

  if(blocks){

    if(blocks > 3){
      blocks = 3;
    }
    int i = 0;
    while(i < blocks){
      sig[i] = pixy.ccc.blocks[i].m_signature;
      if(sig[i]==1){
        red_block = i;
        break;
      }
      else{
        red_block = 10;
      }
      i++;
    }
  }
  else{
    go_straight();
  }
}
void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pixy.init();

}

void loop() {
  Serial.print("red_X:");
  Serial.print(pixy.ccc.blocks[1].m_x);
  Serial.print("\n");
  pixy.ccc.getBlocks();
  blocks = pixy.ccc.numBlocks;
  if(blocks){
    Serial.print("blocks:");
    Serial.print(blocks);
    Serial.print("\n");
    if(pixy.ccc.blocks[1].m_x>200){
      Serial.print("turn right\n");
      turn_right();
    }
    else if(pixy.ccc.blocks[1].m_x<150){

      Serial.print("turn left\n");
      turn_left();
    }
  }
  else
    stop_motor();
    Serial.print("stop\n");
    
  delay(100);
}
  
