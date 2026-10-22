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
const int SERVO_LEFT_PIN = 13;
const int SERVO_RIGHT_PIN = 12;

//condition val
int left;
int right;
int front;

boolean start;

boolean longdead_start;//value for long dead check
unsigned long longdead_start_millis;
unsigned long current_time_millis;
const long longdead_interval = 4000;


int irDetect(int ir_led_pin, int ir_receiver_pin, long frequency){
    tone(ir_led_pin, frequency);
    delay(1);
    int result = digitalRead(ir_receiver_pin);
    return result;
}//return 0 if it detects objects, 1 if it doesn't

int irDistance(int ir_led_pin, int ir_receiver_pin){
    int distance = 0;
    for (long f = 38000; f <= 48000; f += 1100){
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
            current_time_millis = millis();
            if (longdead_start == true && (current_time_millis - longdead_start_millis) > longdead_interval){
                while(true){
                    servoleft.writeMicroseconds(1500);
                    servoright.writeMicroseconds(1500);
                }
            }
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
    if (situation_num != 1 && longdead_start == true){
        longdead_start = false;
    }
    if (start == true){
        delay(1000);
        start = false;
        return;
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
        if (left == 0 && front == 10){
            situation(5);
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1480);
            delay(600);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1440);
            delay(500);
            servoleft.writeMicroseconds(1445);
            servoright.writeMicroseconds(1550);
            delay(1000);
        }
        //bad position to right wall
        else if (right == 0 && front == 10){
            situation(6);
            servoleft.writeMicroseconds(1520);
            servoright.writeMicroseconds(1430);
            delay(700);
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1500);
            delay(450);
            servoleft.writeMicroseconds(1445);
            servoright.writeMicroseconds(1550);
            delay(800);
        }
        //30 degree bad posistion on left
        else if (( left < right ) && front >= 8 && left >= 1 && left <= 5) {
            situation(7);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1560);     
            delay(500);
        }
        //30 degree bad position on right
        else if (( right < left ) && front >= 8 && right >= 1 && right <= 5) {
            situation(8);
            servoleft.writeMicroseconds(1430);
            servoright.writeMicroseconds(1500);
            delay(550);
        //robot is at left corner
        } else if (left <= 3 && front <= 3){
            situation(9);
            servoleft.writeMicroseconds(1445);
            servoright.writeMicroseconds(1550);
            delay(1500);
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1460);
            delay(600);
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1480);
            delay(600);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1440);
            delay(700); 
        //robot is at right corner
        } else if (right <= 3 && front <= 3){
            situation(10);
            servoleft.writeMicroseconds(1445);
            servoright.writeMicroseconds(1550);
            delay(1600);
            servoleft.writeMicroseconds(1540);
            servoright.writeMicroseconds(1430);
            delay(600);
            servoleft.writeMicroseconds(1520);
            servoright.writeMicroseconds(1430);
            delay(600);
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1500);
            delay(500);
        } else {
            situation(0);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1500);
        }
    }
    start = false;
    //turning process (adjustment required)
    if (left == 10 || right == 10) { //there is a path on either left or right.
        if (left == 10){ // if there is a way to left, turn left whenever
            situation(1);
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1443);
            delay(800);
            situation(3);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1400);
            delay(1150);
            situation(1);
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1443);
            delay(800);
        } else if (front >= 9){
            // there is a path in front, going front and follow the left wall
            situation(1);
            longdead_start = false;
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1443);
        } else if (left <= 9 && right == 10 && left >= 4){
            situation(1);
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1443);
            delay(800);
            situation(2);
            servoleft.writeMicroseconds(1600);
            servoright.writeMicroseconds(1500);
            delay(1180);
            situation(1);
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1443);
            delay(800);
        } else {
            situation(0);
            servoleft.writeMicroseconds(1500);
            servoright.writeMicroseconds(1500);
        }
    } else {
        //going straight
        if (front <= 7){
            situation(4);
            //rotate. there is a wall in front and on the both side.
            while ( front != 10 || (left != right) ){
                servoleft.writeMicroseconds(1430);
                servoright.writeMicroseconds(1430);
                front = irDistance(IR_LED_MID, IR_REV_MID);
                left = irDistance(IR_LED_LEFT,IR_REV_LEFT);
                right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);      
            }
            servoleft.writeMicroseconds(1560);
            servoright.writeMicroseconds(1440);
            delay(500);
        } else {
            if (longdead_start == false){ //checking if it's long dead or not by time from the start of long corrider
                longdead_start = true;
                longdead_start_millis = millis();
            }
            if (left == right){ // this means the robots is in the middle of walls.
                situation(1);
                servoleft.writeMicroseconds(1550);
                servoright.writeMicroseconds(1443);
            } else if (left > right){
                situation(1);
                servoleft.writeMicroseconds(1550);
                servoright.writeMicroseconds(1430);
            } else if (right > left){
                situation(1);
                servoleft.writeMicroseconds(1570);
                servoright.writeMicroseconds(1450);
            }
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
    servoleft.attach(SERVO_LEFT_PIN);
    servoright.attach(SERVO_RIGHT_PIN);
    Serial.begin(9600);
    left = 5;
    right = 5;
    start = true;
    longdead_start = false;
    longdead_start_millis = 0;
    current_time_millis = 0;
}

void loop(){
    adjust_move();
}