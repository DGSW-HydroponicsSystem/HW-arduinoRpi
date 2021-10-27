#define DEBUG 1
// LED제어, 워터펌프 제어, 온습도 

#define BAUDRATE 9600

#define DHT11_PIN A0
#define LED_PIN 9
#define WATER_GND_PIN 10
#define WATER_VOL_PIN 11
#define WATER_ENA_PIN 12

#include <DHT.h>
DHT dht = DHT(DHT11_PIN, DHT11);

int ledStat = 0;
int waterStat = 0;

void setup() {
  Serial.begin(BAUDRATE);
  
  dht.begin();

  pinMode(WATER_GND_PIN, OUTPUT);
  pinMode(WATER_VOL_PIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
// LED, 워터펌프 제어
  if(Serial.available()) {
    char c = Serial.read();

    if(c == 'L'){
      digitalWrite(LED_PIN, HIGH);
      ledStat = 1;
    }else if(c == 'l'){
      digitalWrite(LED_PIN, LOW);
      ledStat=0;
    }else if(c == 'W'){
       digitalWrite(WATER_GND_PIN, HIGH);
       digitalWrite(WATER_VOL_PIN, LOW);
       analogWrite(WATER_ENA_PIN, 255);
       waterStat = 1;
    }else if(c == 'w'){
       digitalWrite(WATER_GND_PIN, LOW);
       digitalWrite(WATER_VOL_PIN, LOW);
       analogWrite(WATER_ENA_PIN, 0);
       waterStat = 0;
    }
  }

  // 온습도, LED, 워터펌프 상태 전송
  sendStat();

  delay(500);
}

// 온습도, LED, 워터펌프 상태 전송
void sendStat(){
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

#ifdef DEBUG
  Serial.print("h: ");
  Serial.print(humidity);
  Serial.print(" / t: ");
  Serial.print(temperature);
#else
  Serial.write('H');
  Serial.write((byte*)&humidity, 2);
  Serial.write('T');
  Serial.write((byte*)&temperature, 2);
#endif

#ifdef DEBUG
  Serial.print(" / LED: ");
  Serial.print(ledStat);
  Serial.print(" / Water: ");
  Serial.println(waterStat);
#else
  Serial.write('L');
  Serial.write((byte*)&ledStat, 1);
  Serial.write('W');
  Serial.write((byte*)&waterStat, 1);
  Serial.write('\n');
#endif
}
