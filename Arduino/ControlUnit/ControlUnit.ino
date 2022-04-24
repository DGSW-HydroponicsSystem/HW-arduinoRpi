// ------------------------------------------------------
// Smart Farm Stat Unit Arduino
// https://github.com/ahjeon0407
// ------------------------------------------------------

// Debugging setting
//#define DEBUG 1 

// Baudrate setting
#define BAUDRATE 9600

// Pin setting
#define WATER1_GND_PIN 5
#define WATER1_VOL_PIN 6
#define WATER1_ENA_PIN 7
#define WATER2_GND_PIN 8
#define WATER2_VOL_PIN 9
#define WATER2_ENB_PIN 10
#define LED_PIN 11
#define FAN_PIN 12

int waterPump_stat1 = 0;
int waterPump_stat2 = 0;
int led_stat = 0;
int fan_stat = 0;

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(WATER1_GND_PIN, OUTPUT);
  pinMode(WATER1_VOL_PIN, OUTPUT);
  pinMode(WATER2_GND_PIN, OUTPUT);
  pinMode(WATER2_VOL_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  if(digitalRead(WATER1_GND_PIN)) waterPump_stat1 = 1;
  else if(!digitalRead(WATER1_GND_PIN)) waterPump_stat1 = 0;
  if(digitalRead(WATER2_GND_PIN)) waterPump_stat2 = 1;
  else if(!digitalRead(WATER2_GND_PIN)) waterPump_stat2 = 0;
  if(digitalRead(LED_PIN)) led_stat = 1;
  else if(!digitalRead(LED_PIN)) led_stat = 0;
  if(digitalRead(FAN_PIN)) fan_stat = 1;
  else if(!digitalRead(FAN_PIN)) fan_stat = 0;

  sendStat();
}

void loop() {
  
}

void serialEvent() {
  char msg = Serial.read();

  if(msg == 'W') {
    digitalWrite(WATER1_GND_PIN, LOW);
    digitalWrite(WATER1_VOL_PIN, HIGH);
    digitalWrite(WATER1_ENA_PIN, 255); 
    waterPump_stat1 = 1;   
  }
  else if(msg == 'w') {
    digitalWrite(WATER1_GND_PIN, LOW);
    digitalWrite(WATER1_VOL_PIN, LOW);
    digitalWrite(WATER1_ENA_PIN, 0);    
    waterPump_stat1 = 0;
  }
  else if(msg == 'P') {
    digitalWrite(WATER2_GND_PIN, LOW);
    digitalWrite(WATER2_VOL_PIN, HIGH);
    digitalWrite(WATER2_ENB_PIN, 255); 
    waterPump_stat2 = 1;   
  }
  else if(msg == 'p') {
    digitalWrite(WATER2_GND_PIN, LOW);
    digitalWrite(WATER2_VOL_PIN, LOW);
    digitalWrite(WATER2_ENB_PIN, 0);    
    waterPump_stat2 = 0;
  }
  else if(msg == 'L') {
    digitalWrite(LED_PIN, HIGH);
    led_stat = 1;
  }
  else if(msg == 'l') {
    digitalWrite(LED_PIN, LOW);
    led_stat = 0;
  }
  else if(msg == 'F') {
    digitalWrite(FAN_PIN, HIGH);
    fan_stat = 1;
  }
  else if(msg == 'f') {
    digitalWrite(FAN_PIN, LOW);
    fan_stat = 0;
  }

  sendStat();
}

void sendStat() {
#ifdef DEBUG
  Serial.print("WATER PUMP1: "); Serial.print(waterPump_stat1);
  Serial.print("WATER PUMP2: "); Serial.print(waterPump_stat2);
  Serial.print("LED: "); Serial.print(led_stat);
  Serial.print("FAN: "); Serial.println(fan_stat);
#else
  Serial.write('\x01');
  Serial.write(waterPump_stat1);
  Serial.write(waterPump_stat2);
  Serial.write(led_stat);
  Serial.write(fan_stat);
  Serial.write('\n');
#endif
}
