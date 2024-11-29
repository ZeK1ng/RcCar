#include "nRF24L01.h"
#include "RF24.h"
#include <SPI.h>
#include "Common.h"

#define CE_PIN 7 
#define CSN_PIN 8
const byte  address[6];

RF24 radio(CE_PIN, CSN_PIN);

Cords cords;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  if(radio.available()){
    while(radio.available()){
        radio.read(&cords, sizeof(cords));
    }
    Serial.print("X = ");
    Serial.print(cords.x);
    Serial.print(" Y = ");  
    Serial.println(cords.y);
  }
}
