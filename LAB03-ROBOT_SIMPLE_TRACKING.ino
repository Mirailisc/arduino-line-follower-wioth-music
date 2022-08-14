#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

#define FAR_LEFT_SENSOR_PIN   4
#define LEFT_SENSOR_PIN       5
#define RIGHT_SENSOR_PIN      6
#define FAR_RIGHT_SENSOR_PIN  7

#define MOTOR_LEFT_IN1 11
#define MOTOR_LEFT_IN2 10
#define MOTOR_RIGHT_IN3 9
#define MOTOR_RIGHT_IN4 8


LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo hitServo;

int farLeftSensorValue = 0;
int leftSensorValue = 0;
int rightSensorValue = 0;
int farRightSensorValue = 0;

int tempo = 180;

int buzzer = 12;
int melody[] = {
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  hitServo.attach(3);
  hitServo.write(0);
  delay(250);

  pinMode(FAR_LEFT_SENSOR_PIN, INPUT);
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(FAR_RIGHT_SENSOR_PIN, INPUT);

  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_RIGHT_IN3, OUTPUT);
  pinMode(MOTOR_RIGHT_IN4, OUTPUT);

  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);

  lcd.init();
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Robot Started!");
  delay(1000);
  displaySensor();
}

void readAllSensor() {
  farLeftSensorValue = digitalRead(FAR_LEFT_SENSOR_PIN);
  leftSensorValue = digitalRead(LEFT_SENSOR_PIN);
  rightSensorValue = digitalRead(RIGHT_SENSOR_PIN);
  farRightSensorValue = digitalRead(FAR_RIGHT_SENSOR_PIN);
}

void displaySensor() {
  readAllSensor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Sensor Display");
  lcd.setCursor(0, 1);
  lcd.print(farLeftSensorValue);
  lcd.setCursor(5, 1);
  lcd.print(leftSensorValue);
  lcd.setCursor(10, 1);
  lcd.print(rightSensorValue);
  lcd.setCursor(15, 1);
  lcd.print(farRightSensorValue);
  delay(10);
}
void motorStopAll() {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
}

void forward(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  delay(delay_time);
  //  motorStopAll();
}

void backward(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  delay(delay_time);
  //  motorStopAll();
}

void turnLeft(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  delay(delay_time);
  // motorStopAll();
}

void turnRight(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  delay(delay_time);
  // motorStopAll();
}

void spinLeft(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  delay(delay_time);
  // motorStopAll();
}

void spinRight(int delay_time) {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  delay(delay_time);
  // motorStopAll();
}

int track() {
  displaySensor();
  int delay_time = 50;
  if ((leftSensorValue == 0) && (rightSensorValue == 0)) {
    forward(delay_time);
  } else  if ((leftSensorValue == 0) && (rightSensorValue == 1)) {
    turnRight(delay_time);
  } else  if ((leftSensorValue == 1) && (rightSensorValue == 0)) {
    turnLeft(delay_time);
  } else {
    forward(800);
    motorStopAll();
    return 0;
  }
  return 1;
}

void hit() {
  motorStopAll();
  hitServo.write(45);
  delay(1000);
  hitServo.write(0);
  delay(800);
}

void loop() {

  while (track()); // 1 ทางแยก
  while (track()); // 1 ทางแยก
  spinLeft(800); // เลี้ยวซ้าย

  while (track()); // 1 ทางแยก
  spinRight(600); // เลี้ยวขวา


  while (track()); // 1 ทางแยก
  while (track()); // 1 ทางแยก
  while (track()); // 1 ทางแยก
  spinLeft(800); // เลี้ยวซ้าย
  
  motorStopAll();
  forward(100);
  hit();
  delay(500);
  
  backward(100);
  
  spinRight(1500); // กลับหลังหัน
  while (track()); // 1 ทางแยก
  spinRight(800); // เลี้ยวขวา
  
  motorStopAll();
  backward(200);
  motorStopAll();
  delay(500);
  hit();
  
  backward(100); //ถอยหลัง
  spinLeft(800); // เลี้ยวซ้าย
  
  while (track()); // 1 ทางแยก
  spinLeft(800); // เลี้ยวซ้าย

  while (track()); // 1 ทางแยก
  spinRight(800); // เลี้ยวขวา

  motorStopAll();
  forward(100);
  motorStopAll();
  delay(500);
  hit();
  
  backward(100); //ถอยหลัง
  spinRight(1500); // กลับหลังหัน
  
  while (track()); // 1 ทางแยก
  while (track()); // 1 ทางแยก
  spinRight(800); // เลี้ยวขวา
  
  while (track()); // 1 ทางแยก
  spinLeft(800); // เลี้ยวซ้าย
  motorStopAll();
  forward(100);
  motorStopAll();
  delay(500);
  hit();

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
  
  backward(1000);
  spinRight(3000);
  motorStopAll();
  while (1);
}
