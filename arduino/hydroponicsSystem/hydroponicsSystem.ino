//-----------------------------------------------------------------
// Title    : hydroponicsSystem
// Function : Arduino code to be used in hydroponics system
// History  : 20211013
//-----------------------------------------------------------------

#include "DHT.h"    // "adafruit unified sensor"라이브러리 다운로드

#define LED_PIN  09
#define DHT_PIN  11
#define DHT_TYPE DHT11
#define PUMP_PIN 10

DHT dht(LED_PIN, DHT_TYPE);

int ledFlag = 0;
float humi = 0;
float temp = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(WATR_PIN, INPUT);
}

void loop() {
  dht();
  waterLevel();
  serialRead();
  delay();
}

void dht(){
  Serial.print((int)dht.readHumidity());
  Serial.print((int)dht.readTemperature());
}

int serialRead(){
  
}
