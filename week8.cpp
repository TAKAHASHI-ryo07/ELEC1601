#include <Servo.h>
#include <cstdlib>

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
SERVO_LEFT_PIN = 13;
SERVO_RIGHT_PIN = 12;

//condition val
int left;
int right;
int front;


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
            digitalWrite(A0, LOW);
            digitalWrite(A1, HIGH);
            digitalWrite(A2, LOW);
            delay(1000);
            digitalWrite(A1,LOW);
            break;
        default:
            break;
    }
}

void adjast_move(){
    left = irDistance(IR_LED_LEFT,IR_REV_LEFT); 
    right = irDistance(IR_LED_RIGHT,IR_REV_RIGHT);
    front = irDistance(IR_LED_MID, IR_REV_MID);
    //turning process (required adjastment)
    if (left == 5 || right == 5) { //there is a path on either left or right.
        if (front < 4) { // there is a wall in front
            if (left == 5 && right == 5){
                
            }
            //turn process
        } else {// there is a path in front
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1450);
        }
    } else {
        //going straight
        if (front < 4){
            //rotate. there is a wall in front and on the both side.
            while (front != 5){
                servoleft.writeMicroseconds(1550);
                servoright.writeMilliseconds(1550);
            }
            servoleft.writeMicroseconds(1550);
            servoright.writeMicroseconds(1450);
        }
        else if (left == right){ // this means the robots is in the middle of walls.
            servoleft.writeMilliseconds(1550);
            servoright.writeMilliseconds(1450);
            situation(1);
        } else if (left > right){
            servoleft.writeMilliseconds(1550);
            servoright.writeMilliseconds(1450 - std::abs(left - right) * 10);

        } else {
            servoleft.writeMicroseconds(1550 + std::abs(left - right) * 10);
            servoright.writeMicroseconds(1450);
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
}

void loop(){
    
    adjast_move();
}






