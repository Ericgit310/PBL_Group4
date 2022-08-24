#include <Pixy2.h>
#define ENA 8
#define IN1 9        
#define IN2 10         
#define IN3 11
#define IN4 12
#define ENB 13

//wheel speed
#define speedleft 120
#define speedright 120


int sigOrange = 1;
Pixy2 pixy;

void setup() {
  Serial.begin(115200);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pixy.init();
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
void stop_motor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}


void loop() {
  
  uint16_t blocks;
  while (1){
    blocks = pixy.ccc.getBlocks();
    if(blocks>0){
      int red_block;
      
      red_block = pixy.ccc.blocks[0].m_x;
      

      if(red_block  <= 157){
        turn_left();
        Serial.print("turn left\n");
        
      }else if(red_block >= 158){
        turn_right();
        Serial.print("turn right\n");
      }
      delay(10);
      
    }else{
      stop_motor();
    }
  }
  delay(50);
}

  
