#include "nRF24L01.h"
#include "RF24.h"
#include <SPI.h>
#include "Common.h"

#define CE_PIN 7 
#define CSN_PIN 8
const byte  address[6];
const int DELAY = 1000;
const STICK_X_CENTER = 330;
const STICK_Y_CENTER = 330;
const STICK_MARGIN =  10;


RF24 radio(CE_PIN, CSN_PIN);

struct MotorBridge{
  int motorRightPin1 = 2;
  int motorRightPin2 = 3;
  int motorLeftPin1 = 4;
  int motorLeftPin2 = 5;
};

MotorBridge motorBridge;
Cords cords;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupMotor();
  // setupRadio();
}
void setupMotor(){
  pinMode(motorBridge.motorRightPin1,OUTPUT);
  pinMode(motorBridge.motorRightPin2,OUTPUT);
  pinMode(motorBridge.motorLeftPin1,OUTPUT);
  pinMode(motorBridge.motorLeftPin2,OUTPUT);
  leftStop();
  rightStop();
}

void setupRadio(){
  radio.begin();
  radio.setAutoAck(false);  
  radio.setChannel(CHANNEL_OFFSET);
  radio.setPALevel (RF24_PA_MIN); //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
  radio.openReadingPipe(1,address);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  listenToRadio();
  handleInput();
  delay(DELAY);
}

void printCords(){
  Serial.print("X = ");
  Serial.print(cords.x);
  Serial.print(" Y = ");  
  Serial.println(cords.y);
}

void handleInput(){
  if(cords.x > STICK_X_CENTER - STICK_MARGIN){
    printCords();
    moveForward();
  }else
  if(cords.x < STICK_X_CENTER - STICK_MARGIN){
    printCords();
    moveBackward();
  }else
  if(cords.y > STICK_Y_CENTER + STICK_MARGIN){
    printCords();
    moveRight();
  }else
  if(cords.y < STICK_Y_CENTER - STICK_MARGIN){
    printCords();
    moveLeft();
  }
}

void leftForward(){
  digitalWrite(motorBridge.motorLeftPin1, LOW);
  digitalWrite(motorBridge.motorLeftPin2, HIGH);
}

void rightForward(){
  digitalWrite(motorBridge.motorRightPin1, LOW);
  digitalWrite(motorBridge.motorRightPin2, HIGH);

}
void leftBackward(){
  digitalWrite(motorBridge.motorLeftPin1, HIGH);
  digitalWrite(motorBridge.motorLeftPin2, LOW);
}

void rightBackward(){
  digitalWrite(motorBridge.motorRightPin1, HIGH);
  digitalWrite(motorBridge.motorRightPin2, LOW);
}

void leftStop(){
  digitalWrite(motorBridge.motorLeftPin1, LOW);
  digitalWrite(motorBridge.motorLeftPin2, LOW);
}

void rightStop(){
  digitalWrite(motorBridge.motorRightPin1, LOW);
  digitalWrite(motorBridge.motorRightPin2, LOW);
}

void moveForward(){
  rightForward();
  leftForward();
}

void moveBackward(){
  rightBackward();
  leftBackward();
}

void moveLeft(){
  leftStop();
  rightForward();
}

void moveRight(){
  rightStop();
  leftForward();
}

void listenToRadio(){
  if(radio.available()){
    while(radio.available()){
        radio.read(&cords, sizeof(cords));
    }
    printCords();
  }
}
