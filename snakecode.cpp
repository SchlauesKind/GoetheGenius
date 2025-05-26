#include "LedControl.h"
#include <Wire.h>
#include <Bonezegei_LCD1602_I2C.h>

Bonezegei_LCD1602_I2C lcd(0x27);

LedControl lc = LedControl(0, 2, 1, 1);

int8_t* arrX = nullptr;
int8_t* arrY = nullptr;
const int Ypin = 5;
const int Xpin = 4;
const int SCL_I2C = 15;
const int SDA_I2C = 14;
const int bPin = 22;
long long dur = 0;
long long flashTimer = 0;
long mealX = 0;
long mealY = 0;
int n = 3;
int prevDir = 1;
int speed = 500;
int dir = 0;
int prevdir = 0;
String score = "";
bool flashState = 0;

void setup() {
  pinMode(bPin, OUTPUT);
  pinMode(Xpin, INPUT);
  pinMode(Ypin, INPUT);
  //lcd.begin(16, 2);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  n = 3;
  arrX = new int8_t[n];
  arrY = new int8_t[n];
  arrX[0] = 0;
  arrY[0] = 1;
  generate();
  Serial.begin(115200);

  Wire.begin(SDA_I2C, SCL_I2C);
  lcd.begin();
  lcd.setPosition(0, 0);
  lcd.print("Try to reach 64!");
  lcd.setPosition(0, 1);
  lcd.print("Current score:");
  lcd.setPosition(14, 1);
  score += n;
  lcd.print(score.c_str());
  score = "";
  pipNtimes(2);
}

void pipNtimes(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(bPin, HIGH);
    delay(150);
    digitalWrite(bPin, LOW);
    if (i + 1 < n) delay(150);
  }
}

void newarr(int8_t dir) {
  for (int i = n; i > 0; i--) {
    arrX[i] = arrX[i - 1];
    arrY[i] = arrY[i - 1];
  }
  if (dir == 0) {
    arrX[0]--;
  }
  if (dir == 1) {
    arrY[0]--;
  }
  if (dir == 2) {
    arrX[0]++;
  }
  if (dir == 3) {
    arrY[0]++;
  }
  if (arrX[0] == 8) arrX[0] = 0;
  if (arrY[0] == 8) arrY[0] = 0;
  if (arrX[0] == -1) arrX[0] = 7;
  if (arrY[0] == -1) arrY[0] = 7;
}

void disarr() {
  lc.clearDisplay(0);
  for (int i = 0; i < n; i++) {
    lc.setLed(0, arrX[i], arrY[i], true);
  }
}

void disEat(){
  int del = 200;
  if (flashState) del = 50;
  if (millis() - flashTimer > del) {
    lc.setLed(0, mealX, mealY, flashState);
    flashState = !flashState;
    flashTimer = millis();
  }
}

void deb(int n) {
  Serial.println("------------------");
  Serial.print(arrX[n]);
  Serial.print(" ");
  Serial.println(arrY[n]);
}

bool ifselfkill() {
  for (int i = 1; i < n; i++) {
    if (arrX[0] == arrX[i] && arrY[0] == arrY[i]) return true;
  }
  return false;
}

void game_over() {
  pipNtimes(3);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, true);
      delay(20);
    }
  }
  //delay(100);
  lc.clearDisplay(0);
  lc.setRow(0, 0, B00000000);
  lc.setRow(0, 1, B00011000);
  lc.setRow(0, 2, B11100000);
  lc.setRow(0, 3, B01100000);
  lc.setRow(0, 4, B11001111);
  lc.setRow(0, 5, B11110000);
  lc.setRow(0, 6, B00000000);
  lc.setRow(0, 7, B00000000);
  delay(300);
  pipNtimes(2);
  lc.clearDisplay(0);
  lc.setRow(0, 0, B00000000);
  lc.setRow(0, 1, B00000000);
  lc.setRow(0, 2, B01011000);
  lc.setRow(0, 3, B10100000);
  lc.setRow(0, 4, B11001111);
  lc.setRow(0, 5, B11110000);
  lc.setRow(0, 6, B00000000);
  lc.setRow(0, 7, B00000000);
  delay(300);
  pipNtimes(2);
  lc.clearDisplay(0);
  lc.setRow(0, 0, B00000000);
  lc.setRow(0, 1, B00000000);
  lc.setRow(0, 2, B01000000);
  lc.setRow(0, 3, B10111000);
  lc.setRow(0, 4, B11001111);
  lc.setRow(0, 5, B11110000);
  lc.setRow(0, 6, B00000000);
  lc.setRow(0, 7, B00000000);
  delay(300);
  pipNtimes(2);
  lc.clearDisplay(0);
  lc.setRow(0, 0, B00000000);
  lc.setRow(0, 1, B00000000);
  lc.setRow(0, 2, B01000000);
  lc.setRow(0, 3, B10100000);
  lc.setRow(0, 4, B11111111);
  lc.setRow(0, 5, B11110000);
  lc.setRow(0, 6, B00000000);
  lc.setRow(0, 7, B00000000);
  delay(500);
  lc.clearDisplay(0);
  n = 3;
  arrX = new int8_t[n];
  arrY = new int8_t[n];
  arrX[0] = 0;
  arrY[0] = 1;
  //game_intro();
  lcd.setPosition(14, 1);
  lcd.print("  ");
  lcd.setPosition(14, 1);
  score = "";
  score += n;
  lcd.print(score.c_str());
}

void generate() {
  mealX = random(8);
  mealY = random(8);
  for (int i = 1; i < n; i++) {
    if (mealX == arrX[i] && mealY == arrY[i]) generate();
  }
}

void eaten() {
  if (mealX == arrX[0] && mealY == arrY[0]) {
    n++;
    int8_t* tX = nullptr;
    int8_t* tY = nullptr;
    tX = new int8_t[n];
    tY = new int8_t[n];
    for (int i = 0; i < n - 1; i++) {
      tX[i] = arrX[i];
      tY[i] = arrY[i];
    }
    arrX = new int8_t[n];
    arrY = new int8_t[n];
    for (int i = 0; i < n - 1; i++) {
      arrX[i] = tX[i];
      arrY[i] = tY[i];
    }
    delete[] tX;
    delete[] tY;
    lcd.setPosition(14, 1);
    score += n;
    lcd.print(score.c_str());
    score = "";
    pipNtimes(1);
    generate();
  }
}

void loop() {
  speed = analogRead(3) / 5;
  int inX = analogRead(Xpin);
  int inY = analogRead(Ypin);
  if (inX > 3300) {
    if (prevdir != 0)
      dir = 2;
  }
  if (inX < 10) {
    if (prevdir != 2)
      dir = 0;
  }
  if (inY > 3300) {
    if (prevdir != 3)
      dir = 1;
  }
  if (inY < 10) {
    if (prevdir != 1)
      dir = 3;
  }
  if ((millis() - dur) > speed) {
    dur = millis();
    prevdir=dir;
    newarr(dir);
    disarr();
  }
  if (ifselfkill()) game_over();
  eaten();
  disEat();
}