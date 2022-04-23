// ------------------------------------------------------
// Smart Farm Stat Unit Arduino
// https://github.com/ahjeon0407
// ------------------------------------------------------

// Debugging setting
#define DEBUG 1

// Baudrate setting
#define BAUDRATE 9600

// Pin setting
#define WATER_GND_PIN 5
#define WATER_VOL_PIN 6
#define WATER_ENA_PIN 7
#define LED_PIN 8
#define FAN_PIN 9

int waterPump_stat = 0;
int led_stat = 0;
int fan_stat = 0;

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(WATER_GND_PIN, OUTPUT);
  pinMode(WATER_VOL_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
  
}

void SerialEvent() {
  char msg = Serial.read();

  if(msg == 'W') {
    digitalWrite(WATER_GND_PIN, HIGH);
    digitalWrite(WATER_VOL_PIN, LOW);
    digitalWrite(WATER_ENA_PIN, 255); 
    waterPump_stat = 1;   
  }
  else if(msg == 'w') {
    digitalWrite(WATER_GND_PIN, LOW);
    digitalWrite(WATER_VOL_PIN, LOW);
    digitalWrite(WATER_ENA_PIN, 0);    
    waterPump_stat = 0;
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
  Serial.print("WATER PUMP: "); Serial.print(waterPump_stat);
  Serial.print("LED: "); Serial.print(led_stat);
  Serial.print("FAN: "); Serial.println(fan_stat);
#else
  Serial.write('1');              Serial.write('/');
  Serial.write(waterPump_stat);   Serial.write('/');
  Serial.write(led_stat);         Serial.write('/');
  Serial.write(fan_stat);         Serial.write('/');
  Serial.write('\n')
#endif
}
