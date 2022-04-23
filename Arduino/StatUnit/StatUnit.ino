// ------------------------------------------------------
// Smart Farm Stat Unit Arduino
// https://github.com/ahjeon0407
// ------------------------------------------------------
// 필요 라이브러리 (라이브러리 관리자에서 설치)
// DHT sensor library (by Adafruit)
// OneWire (by Paul Stoffregen)
// Dallas Temperature (by Miles Burton)

// Debugging setting
#define DEBUG 1

// Baudrate setting
#define BAUDRATE 9600

// Sensor Pin Settings
#define DHT11_PIN1      A4
#define DHT11_PIN2      A3
#define CDS_PIN1        A2  // analog pin
#define CDS_PIN2        A1  // analog pin
#define WATERTEMP_PIN1  12  // digital pin
#define WATERTEMP_PIN2  13  // digital pin
#define PH_PIN          A0  // analog pin

// DHT11
#include <DHT.h>
DHT dht1 = DHT(DHT11_PIN1, DHT11);
DHT dht2 = DHT(DHT11_PIN2, DHT11);

// Water Temp
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire ds1(WATERTEMP_PIN1);
OneWire ds2(WATERTEMP_PIN2);
DallasTemperature wT1(&ds1);
DallasTemperature wT2(&ds2);

// Water Level
int waterLevel_Pin[10] = {2,3,4,5,6,7,8,9,10,11};

void setup() {
  Serial.begin(BAUDRATE);

  dht1.begin();
  dht2.begin();
  wT1.begin();
  wT2.begin();
  for(int i = 0; i < 10; i++)  {
    pinMode(waterLevel_Pin[i], INPUT);
  }
  
}

// -------------------------------
int temperature1 = 0;    // 1층 온도
int humidity1 = 0;       // 1층 습도
int temperature2 = 0;    // 2층 온도
int humidity2 = 0;       // 2층 습도
int sunshine1 = 0;       // 1층 일조량
int sunshine2 = 0;       // 2층 일조량
int waterTemp1 = 0;      // 1층 수온
int waterTemp2 = 0;      // 2층 수온
int waterLevel = 0;     // 물높이
float waterPH = 0;        // 물 산성도
// -------------------------------

void loop() {
  wT1.requestTemperatures();
  wT2.requestTemperatures();

  sensor();
  sendStat();
  delay(100);
}

void sensor() {
  temperature1 = dht1.readTemperature();
  humidity1 = dht1.readHumidity();
  temperature2 = dht2.readTemperature();
  humidity2 = dht2.readHumidity();
  sunshine1 = analogRead(CDS_PIN1);
  sunshine2 = analogRead(CDS_PIN2);
  waterTemp1 = wT1.getTempCByIndex(0);
  waterTemp2 = wT2.getTempCByIndex(0);
  waterLevel = waterLevelMeasurement();
  waterPH = waterPHMeasurement();
}

int waterLevelMeasurement() {
  int level = 0;
  for(int i = 0; i < 10; i++) {
    if(digitalRead(waterLevel_Pin[i]) == HIGH)
      level++;
    else
      break;
  }
  return level;
}

float waterPHMeasurement() {
  int cnt = 10;
  float adc_resolution = 1024.0;
  int sum = 0;

  for(int i  = 0; i < cnt; i++) {
    sum += analogRead(PH_PIN);
    delay(10);
  }

  float voltage = 5/adc_resolution * sum/cnt;
  float ph = 7+((2.5 - voltage)/0.18);
  return ph;
}

void sendStat() {
#ifdef DEBUG
  Serial.print("t1:");  Serial.print(temperature1);
  Serial.print("h1:");  Serial.print(humidity1);
  Serial.print("t2:");  Serial.print(temperature2);
  Serial.print("h2:");  Serial.print(humidity2);
  Serial.print("s1:");  Serial.print(sunshine1);
  Serial.print("s2:");  Serial.print(sunshine2);
  Serial.print("wT1:");  Serial.print(waterTemp1);
  Serial.print("wT2:");  Serial.print(waterTemp2);
  Serial.print("wL:");  Serial.print(waterLevel);
  Serial.print("ph:");  Serial.print(waterPH);
  Serial.println();
#else
  Serial.write('0');            Serial.write('/');
  Serial.write(temperature1);   Serial.write('/');
  Serial.write(humidity1);      Serial.write('/');
  Serial.write(temperature2);   Serial.write('/');
  Serial.write(humidity2);      Serial.write('/');
  Serial.write(sunshine1);      Serial.write('/');
  Serial.write(sunshine2);      Serial.write('/');
  Serial.write(waterTemp1);     Serial.write('/');
  Serial.write(waterTemp2);     Serial.write('/');
  Serial.write(waterLevel);     Serial.write('/');
  Serial.write(waterPH);        Serial.write('/');
  Serial.write('\n');
#endif
}
