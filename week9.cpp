#include <Servo.h>

//mid senser
const int IR_LED_MID = 6;
const int IR_REV_MID = 7;
const int RED_LED_MID = A1;

//right senser
const int IR_LED_RIGHT = 2;
const int IR_REV_RIGHT = 3;
const int RED_LED_RIGHT = A0;

//left senser
const int IR_LED_LEFT = 10;
const int IR_REV_LEFT = 11;
const int RED_LED_LEFT = A2;

//Servo
Servo servoleft;
Servo servoright;
const int SERVO_LEFT_PIN = 12;
const int SERVO_RIGHT_PIN = 13;

//condition val
int left;
int right;
int front;

boolean start;


int irDetect(int ir_led_pin, int ir_receiver_pin, long frequency){
    tone(ir_led_pin, frequency);
    delay(1);
    int result = digitalRead(ir_receiver_pin);
    return result;
}//return 0 if it detects objects, 1 if it doesn't

int irDistance(int ir_led_pin, int ir_receiver_pin){
    int distance = 0;
    for (long f = 38000; f <= 42000; f += 1000){
        distance += irDetect(ir_led_pin, ir_receiver_pin, f);
    }
    noTone(ir_led_pin);
    return distance;
}//the bigger the distance is, the farther the object is

void situation(int situation_num){
    switch (situation_num){
        case 0:
            digitalWrite(A0, LOW);
            digitalWrite(A1, LOW);
            digitalWrite(A2, LOW);
            break;

        case 1:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, LOW);
            digitalWrite(A2, LOW);
            break;
        case 2:
            digitalWrite(A0, LOW);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, LOW);
            break;
        case 3:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, LOW);
            break;
        case 4:
            digitalWrite(A0, LOW);
            digitalWrite(A1, LOW);
            digitalWrite(A2, HIGH);
            break;
        case 5:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, LOW);
            digitalWrite(A2, HIGH);
            break;
        case 6:
            digitalWrite(A0, LOW);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, HIGH);
            break;
        case 7:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, HIGH);
            break;
        case 8:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, LOW);
            digitalWrite(A2, LOW);
            delay(1000);
            digitalWrite(A0,LOW);
            break;
        case 9:
            digitalWrite(A0, LOW);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, LOW);
            delay(1000);
            digitalWrite(A1,LOW);
            break;
        case 10:
            digitalWrite(A0, HIGH);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, LOW);
            delay(1000);
            digitalWrite(A0,LOW);
            digitalWrite(A1,LOW);
            break;
        default:
            break;
    }
}

void adjust_move(){
    left = irDistance(IR_LED_LEFT,IR_REV_LEFT);
    right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);
    front = irDistance(IR_LED_MID, IR_REV_MID);
    Serial.println(left);
    Serial.println(right);
    Serial.println(front);
    //starting position
    if (start == true) {
      //bad position to left wall
      if (left <= 1 && front == 5 && right <= 5){
        situation(5);
        servoleft.writeMicroseconds(1560);
        servoright.writeMicroseconds(1460);
        delay(500);
        servoleft.writeMicroseconds(1560);
        servoright.writeMicroseconds(1480);
        delay(500);
        servoleft.writeMicroseconds(1500);
        servoright.writeMicroseconds(1440);
        delay(650);
        servoleft.writeMicroseconds(1445);
        servoright.writeMicroseconds(1550);
        delay(1000);
        }
      //bad position to right wall
      else if (right <= 1 && front == 5 && left <= 5){
        situation(6);
        servoleft.writeMicroseconds(1540);
        servoright.writeMicroseconds(1430);
        delay(500);
        servoleft.writeMicroseconds(1520);
        servoright.writeMicroseconds(1430);
        delay(500);
        servoleft.writeMicroseconds(1560);
        servoright.writeMicroseconds(1500);
        delay(450);
        servoleft.writeMicroseconds(1445);
        servoright.writeMicroseconds(1550);
        delay(900);
      }
      else if (( left < right ) && (front < right) && front != 5) {
        situation(7);
        while ( front != 5 ){
          servoleft.writeMicroseconds(1500);
          servoright.writeMicroseconds(1560);
          front = irDistance(IR_LED_MID, IR_REV_MID);
          left = irDistance(IR_LED_LEFT,IR_REV_LEFT);
          right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);      
        }
        delay(500);
      }
      else if (( right < left ) && (front < left) && front != 5) {
        situation(8);
        while ( front != 5 ){
          servoleft.writeMicroseconds(1440);
          servoright.writeMicroseconds(1500);
          front = irDistance(IR_LED_MID, IR_REV_MID);
          left = irDistance(IR_LED_LEFT,IR_REV_LEFT);
          right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);      
        }
        delay(500);
      }
    }
    start = false;
    //turning process (adjustment required)
    if (left == 5 || right == 5) { //there is a path on either left or right.
        if (front < 2) { // there is a wall in front
            if (left == 5 && right == 5){
                servoleft.writeMicroseconds(1480);
                servoright.writeMicroseconds(1480);
                delay(3000);
            }
            //turn left at an ideal position
            else if (right < 4 && left == 5 && right > 1){
                situation(3);
                servoleft.writeMicroseconds(1480);
                servoright.writeMicroseconds(1480);
                delay(3000);
            }
            //
            else if (left < 4 && right == 5 && left > 1){
                situation(2);
                servoleft.writeMicroseconds(1520);
                servoright.writeMicroseconds(1520);
                delay(2000);
            }
            //turn left at a bad 
            else if (right < 1 && left == 5) {
                situation(9);
            }
            else {
              situation(0);
              servoleft.writeMicroseconds(1500);
              servoright.writeMicroseconds(1500);
            }
            
            
        } else {// there is a path in front
            situation(1);
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1440);
        }
    } else {
        //going straight
        if (front < 2){
            situation(4);
            //rotate. there is a wall in front and on the both side.
            while ( front != 5 || (left != right) ){
                servoleft.writeMicroseconds(1400);
                servoright.writeMicroseconds(1400);
                front = irDistance(IR_LED_MID, IR_REV_MID);
                left = irDistance(IR_LED_LEFT,IR_REV_LEFT);
                right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);      
            }
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1440);
            delay(1000);
        }
        else if (left == right){ // this means the robots is in the middle of walls.
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1440);
            situation(1);
        } else if (left > right){
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1430);
            situation(1);
        } else {
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1440);
            situation(1);
        }
    }
}

void setup(){
    pinMode(IR_LED_MID, OUTPUT);
    pinMode(IR_REV_MID, INPUT);
    pinMode(RED_LED_MID, OUTPUT);
    pinMode(IR_LED_RIGHT, OUTPUT);
    pinMode(IR_REV_RIGHT, INPUT);
    pinMode(RED_LED_RIGHT, OUTPUT);
    pinMode(IR_LED_LEFT, OUTPUT);
    pinMode(IR_REV_LEFT, INPUT);
    pinMode(RED_LED_LEFT, OUTPUT);
    servoleft.attach(SERVO_RIGHT_PIN);
    servoright.attach(SERVO_LEFT_PIN);
    Serial.begin(9600);
    left = 5;
    right = 5;
    start = true;
}

void loop(){
    adjust_move();
}
