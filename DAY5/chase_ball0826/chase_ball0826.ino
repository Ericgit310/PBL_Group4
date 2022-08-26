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
uint16_t blocks;
int i = 0;
int sig[3], x[3], width[3];
int red_block, white_block;
int startTime;
void stop_motor() {
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
void collect_redball() {
  int angle = 0;
  stop_motor();
  myservo1.write(angle);
  if (angle < 90) {
    for (; angle <90 ; angle += 1) {
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
  myservo1.attach(servo1);
  pixy.init();
  myservo1.write(0);
  startTime = millis();
}

void loop() {
  Serial.print("\n");
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
    //(pixy.ccc.blocks[0].m_width >= 90)
    if ((pixy.ccc.blocks[0].m_x > 140) && (pixy.ccc.blocks[0].m_x < 210) && (pixy.ccc.blocks[0].m_y > 180)) {
      Serial.print("collect redball\n");
      stop_motor();
      delay(500);
//      go_straight();
//      delay(300);
      collect_redball();
    }
    else if (pixy.ccc.blocks[0].m_x > 140 && pixy.ccc.blocks[0].m_x < 210 && pixy.ccc.blocks[0].m_width < 180) {
      Serial.print("go straight\n");
      go_straight();
      delay(50);
      stop_motor();
    }
    else if (pixy.ccc.blocks[0].m_x > 210) {
      Serial.print("turn right\n");
      turn_right();
      delay(50);
      stop_motor();
      delay(100);
    }
    else if (pixy.ccc.blocks[0].m_x < 140){
      Serial.print("turn left\n");
      turn_left();
      delay(50);
      stop_motor();
      delay(100);
    }
  }
  else if((millis()-startTime)>500){
    Serial.print((millis()-startTime));
    startTime = millis();
    Serial.print("turn right search ball");
    turn_right(40,56);
    delay(200);
    stop_motor();
    delay(100);
  }
  else{
    detect_redball();
    Serial.print("detect_redball\n");
  }
}
