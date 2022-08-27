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
#define speedleft 50
#define speedright 66
#define servo1 5
#define R_trigger 30
#define R_echo 31
#define C_trigger 26
#define C_echo 27
int turnright_speedright;
int turnright_speedleft;
int turnleft_speedright;
int turnleft_speedleft;
uint16_t blocks;
int i = 0;
int sig[3], x[3], width[3];
int red_block, white_block;
int startTime;
int missiontime;
int L_cm = 0, R_cm = 0, C_cm = 0;
long R_duration, L_duration, C_duration;
const int turn_Distance = 20;
bool startup = 0;
void stop_motor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void collect_redball() {
  int angle = 0;
  stop_motor();
  myservo1.write(angle);
  if (angle < 130) {
    for (; angle <130  ; angle += 1) {
      myservo1.write(angle);
      delay(5);
    }
  }
  delay(1000);
  if (angle > 0) {
    for (; angle > 0; angle -= 1) {
      myservo1.write(angle);
      delay(5);
    }
  }
  delay(1000);
}
void go_straight(int left = speedleft, int right = speedright) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, left);
  analogWrite(ENB, right);
}

void go_back() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedleft);
  analogWrite(ENB, speedright);
}

void turn_right(int left = speedleft, int right = speedright) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, left);
  analogWrite(ENB, right);
}

void turn_left(int left = speedleft, int right = speedright) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, left);
  analogWrite(ENB, right);

}
void detect_redball() {
  pixy.ccc.getBlocks();
  blocks = pixy.ccc.numBlocks;

  if (blocks) {

    if (blocks > 3) {
      blocks = 3;
    }
    int i = 0;
    while (i < blocks) {
      sig[i] = pixy.ccc.blocks[i].m_signature;
      if (sig[i] == 1) {
        red_block = i;
        break;
      }
      else {
        red_block = 10;
      }
      i++;
    }
  }
  else {
    stop_motor();
  }
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
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(servo1, OUTPUT);
  pinMode(R_trigger, OUTPUT);
  pinMode(C_trigger, OUTPUT);  
  pinMode(R_echo, INPUT);
  pinMode(C_echo, INPUT);
  pinMode(servo1, OUTPUT);
  myservo1.attach(servo1);
  pixy.init();
  myservo1.write(0);
  startTime = millis();
  collect_redball();
}

void loop() {
  missiontime=millis();
  while((millis()-missiontime)< 6000000){
    Serial.print("mission 1:        ");
    //ball detection
    pixy.ccc.getBlocks();
    blocks = pixy.ccc.numBlocks;
  if (blocks) {
    for (i = 0; i < 1; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print("red_X:");
      Serial.println(pixy.ccc.blocks[i].m_x);
      Serial.print(": ");
      Serial.print("red_WIDTH:");
      Serial.println(pixy.ccc.blocks[i].m_width);
    }
    Serial.print("blocks:");
    Serial.print(blocks);
    Serial.print("\n");
    turnright_speedright= (((pixy.ccc.blocks[0].m_x - 157)*56)/157) +50;
    turnright_speedleft= (((pixy.ccc.blocks[0].m_x - 157) *40)/157) +50;
    turnleft_speedright= (((157 - pixy.ccc.blocks[0].m_x)*56)/157) +50;
    turnleft_speedleft= (((157 - pixy.ccc.blocks[0].m_x) *40)/157) +50;
    //(pixy.ccc.blocks[0].m_width >= 90)
    if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x > 140) && (pixy.ccc.blocks[0].m_x < 210) && (pixy.ccc.blocks[0].m_y > 190 )) {
      Serial.print("collect redball\n");
      stop_motor();
      delay(500);
      go_straight();
      delay(150);
      collect_redball();
      
    }
    else if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x > 140 && pixy.ccc.blocks[0].m_x < 210 && pixy.ccc.blocks[0].m_width < 180)) {
      Serial.print("go straight\n");
      go_straight();
      delay(50);
      stop_motor();
    }
    else if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x > 177)) {

      Serial.print("turn right\n");
      Serial.println(turnright_speedleft);
      Serial.println(turnright_speedright);
      
      turn_right(turnright_speedleft, turnright_speedright);
      delay(100);
      stop_motor();
      delay(100);
    }
    else if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x < 137)){
      Serial.print("turn left\n");
      Serial.println(turnleft_speedleft);
      Serial.println(turnleft_speedright);
      turn_left(turnleft_speedleft, turnleft_speedright);
      delay(100);
      stop_motor();
      delay(100); 
    }
  }
  else if((millis()-startTime)>500){
    Serial.print((millis()-startTime));
    startTime = millis();
    Serial.println("turn right search ball");
    turn_right(50,66);
    delay(200);
    stop_motor();
    delay(100);
  }
  else{
    detect_redball();
    Serial.println("detect_redball");
    }
  }
  /////////////////////////////////////////////mission 2///////////////////////////////////////////////////////////////////
  startTime = millis();
  while(1){    
    Serial.println("mission 2");
    //sonic senor
    Distance();
    if(C_cm < turn_Distance-8){
      Serial.println("C_sonic detected");
      go_back();
      delay(1000);
      turn_left();
      delay(500);
      stop_motor();
      delay(1000);
      startup =1;
      if(startup == 1){
        go_straight(30,0);
        delay(200);
        }
      }
    else if(R_cm < turn_Distance-2){
        Serial.println("R_sonic detected");
        turn_left(30,30);
        delay(75);
        stop_motor();
      }
    pixy.ccc.getBlocks();
    blocks = pixy.ccc.numBlocks;
    //ball detection
    if (blocks) {
      for (i = 0; i < 1; i++)
      {
        Serial.print("  block ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print("red_X:");
        Serial.println(pixy.ccc.blocks[i].m_x);
        Serial.print(": ");
        Serial.print("red_WIDTH:");
        Serial.println(pixy.ccc.blocks[i].m_width);
      }
      Serial.print("blocks:");
      Serial.print(blocks);
      Serial.print("\n");
      //(pixy.ccc.blocks[0].m_width >= 90)
      if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x > 140) && (pixy.ccc.blocks[0].m_x < 210) && (pixy.ccc.blocks[0].m_y > 180)) {
        Serial.println("collect redball\n");
        stop_motor();
        delay(500);
        go_straight(60,76);
        delay(300);
        collect_redball();
      }
      else if ((pixy.ccc.blocks[0].m_y > 30)&& pixy.ccc.blocks[0].m_x > 140 && pixy.ccc.blocks[0].m_x < 210 && pixy.ccc.blocks[0].m_width < 180) {
        Serial.println("go straight\n");
        go_straight(80,96);
        delay(100);
        stop_motor();
      } 
      else if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x > 210)) {
        Serial.println("turn right\n");
        turn_right(80, 96);
        delay(100);
        stop_motor();
        delay(100);
      }
      else if ((pixy.ccc.blocks[0].m_y > 30)&&(pixy.ccc.blocks[0].m_x < 140)){
        Serial.println("turn left\n");
        turn_left(80,96);
        delay(100);
        stop_motor();
        delay(100);
      }
    }
    else if((millis()-startTime)>500){
      Serial.println((millis()-startTime));
      
      Serial.println("go straight search ball");
      go_straight(80,96);
      if((millis()-startTime)>1000){
        stop_motor();
        startTime = millis();
      }
    }
    else{
      detect_redball();
      Serial.println("detect_redball\n");
    }
  }
}
