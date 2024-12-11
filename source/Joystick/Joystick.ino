#include "nRF24L01.h"
#include "RF24.h"
#include <SPI.h>
#include "Common.h"


#define CE_PIN  9
#define CSN_PIN 10

const byte address[6] = "00001"; 

struct JoystickPinConfig {
  int x_axis = A0;
  int y_axis = A1;
  int up = 2;
  int down = 4;
  int left = 5;
  int right = 3;
};

JoystickPinConfig joystickPinConfig;

struct Joystick{
  int x_axis;
  int y_axis;
  int up;
  int down;
  int left;
  int right;
};

Joystick joystick;

Cords cords;

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(9600); 
  setupRadio();
	setupJoystick();
}
void setupRadio(){
  radio.begin(); 
  radio.setAutoAck(false);  
  radio.setChannel(CHANNEL_OFFSET);
  radio.openWritingPipe(address); 
  radio.setPALevel (RF24_PA_LOW); //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.setDataRate(RF24_1MBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
  radio.stopListening(); 
}

void setupJoystick(){
  pinMode(joystickPinConfig.up, INPUT_PULLUP);
  pinMode(joystickPinConfig.down, INPUT_PULLUP);
  pinMode(joystickPinConfig.left, INPUT_PULLUP);
  pinMode(joystickPinConfig.right, INPUT_PULLUP);
  
  digitalWrite(joystickPinConfig.up, LOW);
  digitalWrite(joystickPinConfig.down, LOW);
  digitalWrite(joystickPinConfig.left, LOW);
  digitalWrite(joystickPinConfig.right, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
    printInputs();
    radio.write(&cords, sizeof(cords));
    delay(200);
}

void printInputs(){
  cords.x = analogRead(joystickPinConfig.x_axis);
  cords.y = analogRead(joystickPinConfig.y_axis);
  Serial.print("X = ");
  Serial.print(cords.x);
  Serial.print(" Y = ");  
  Serial.println(cords.y);
}
