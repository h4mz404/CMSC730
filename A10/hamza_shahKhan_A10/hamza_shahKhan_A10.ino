/*
CMSC730 Assignment 10
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/BNUXRfWf5U0
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

#define buttonPin_UP 18
#define buttonPin_DWN 19
#define buttonPin_LFT 5
#define buttonPin_RHT 17
#define buttonPin_MID 16
#define buttonPin_SET 4 
#define buttonPin_RST 0 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


                                    
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int x_pos = SCREEN_WIDTH/2;
int y_pos = SCREEN_HEIGHT/2;
bool screen_invert = false;

const int MPU = 0x68;

const int sda = 21;
const int scl = 22;

byte ACCEL_OUT_H = 0;
byte ACCEL_OUT_L = 0;
int16_t ACCEL_RAW = 0;
float accX,accY,accZ;
int goalX1 = 0, goalY1 = 0;
int goalX2 = 0, goalY2 = 0;
float start_time, end_time, run_time;

int targetsHit = 0;
int level_select = 1;
bool goal1_flag = false;
bool goal2_flag = false;

int buzzerPin = 12;  

void setup() {
  //Pin Initialization
  pinMode(buttonPin_UP, INPUT_PULLUP);
  pinMode(buttonPin_DWN, INPUT_PULLUP);
  pinMode(buttonPin_LFT, INPUT_PULLUP);
  pinMode(buttonPin_RHT, INPUT_PULLUP);
  pinMode(buttonPin_MID, INPUT_PULLUP);
  pinMode(buttonPin_SET, INPUT_PULLUP);
  pinMode(buttonPin_RST, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  //Comms Initialization
  Serial.begin(9600);
  Wire.begin(sda, scl); //SDA, SCL
  //OLED Init
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("");
  display.setTextSize(3);
  display.println("AimNHit");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Game");
  display.setTextSize(3);
  display.println("START!");
  display.display();
  delay(1000);

  selectLevel();
  randomSeed(analogRead(19));

  genRanPoint(goalX1,goalY1);
  genRanPoint(goalX2,goalY2);
  display.clearDisplay();
  display.drawPixel(goalX1, goalY1, SSD1306_WHITE);
  display.display();
  }

void loop(){
  start_time = millis();
  while(targetsHit < 10){
    if(goal1_flag){
      genRanPoint(goalX1,goalY1);
      goal1_flag = false;
    }
    if(goal2_flag){
      genRanPoint(goalX2,goalY2);
      goal2_flag = false;
    }
    while(!goal1_flag && !goal2_flag){
      updatePos();
      checkButton();
      checkBoundary();
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println(targetsHit);
      display.drawPixel(goalX1, goalY1, SSD1306_WHITE);
      if(level_select == 2){display.drawPixel(goalX2, goalY2, SSD1306_WHITE);
      checkGoal(goalX2, goalY2, goal2_flag);}
      display.drawPixel(x_pos, y_pos, SSD1306_WHITE);
      display.display();
      delay(10);
      checkGoal(goalX1, goalY1, goal1_flag);
    }
  }
  end_time = millis();
  while(targetsHit >= 10){
    displayWinScreen();
    winTone();
  }
}

void displayWinScreen(){
  run_time = (end_time - start_time)/1000;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Game");
  display.setTextSize(3);
  display.println("WON!");
  display.setTextSize(1);
  String time_elapsed = "Time:" +String(run_time) + "s";
  display.println(time_elapsed);
  display.display();
  delay(100);
  String msg = "WON!" ;
  for(int i=0;i<3;i++){
    display.setTextSize(3);
    display.setCursor(0, 17);
    msg = msg + "!";
    display.println(msg);
    display.display();
    delay(100);
  }
}

void selectLevel(){
  Serial.println("Please Select Level");
  while(digitalRead(buttonPin_MID) != LOW){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Select Lvl");
    display.setTextSize(3);
    display.setCursor(0, 20);
    String msg = "Level " + String(level_select);
    display.println(msg);
    display.display();
    if (digitalRead(buttonPin_RHT) == LOW) {level_select = 2;}
    if (digitalRead(buttonPin_LFT) == LOW) {level_select = 1;}
  }
  Serial.print("Level Selected :");
  Serial.println(level_select);
}

void updatePos(){
  accY = getData(0x3B);
  accX = getData(0x3D);
  if (accY > 0.1) {x_pos +=1;}
  if (accY <-0.1) {x_pos -=1;}
  if (accX >0.1)  {y_pos -=1;}
  if (accX <-0.1) {y_pos +=1;}
}

float getData(int address){
  Wire.beginTransmission(MPU);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 2);
  ACCEL_OUT_H = Wire.read();
  ACCEL_OUT_L = Wire.read();
  ACCEL_RAW = ACCEL_OUT_H << 8 | ACCEL_OUT_L;
  return ACCEL_RAW / 16384.0;
}

void genRanPoint(int &goalX, int &goalY){
  int ran_x = random(127);
  int ran_y = random(17,63);
  goalX = ran_x;
  goalY = ran_y;
  Serial.print("New Goal: ");
  Serial.print(goalX);
  Serial.print(",");
  Serial.println(goalY);
}

void checkBoundary(){
  if(x_pos < 0){x_pos = 0;}
  if(x_pos > SCREEN_WIDTH-1){x_pos = SCREEN_WIDTH-1;}
  if(y_pos < 0){y_pos = 0;}
  if(y_pos > SCREEN_HEIGHT-1){y_pos = SCREEN_HEIGHT-1;}
} 

void checkGoal(int goalX, int goalY, bool &goal_flag){
  int clr = 4;
  if(x_pos>goalX-clr && x_pos<goalX+clr && y_pos>goalY-clr && y_pos<goalY+clr){
    goal_flag = true;
    targetsHit +=1;
    Serial.print("Targets Hit:");
    Serial.println(targetsHit);
    screen_invert = !screen_invert;
    display.invertDisplay(screen_invert);
    display.display();
    screen_invert = !screen_invert;
    display.invertDisplay(screen_invert);
    display.display();
    digitalWrite(buzzerPin,HIGH);
    delay(50);
    digitalWrite(buzzerPin,LOW);
  }
}

void checkButton(){
  if (digitalRead(buttonPin_RHT) == LOW) {x_pos +=1;}
  if (digitalRead(buttonPin_LFT) == LOW) {x_pos -=1;}
  if (digitalRead(buttonPin_UP) == LOW)  {y_pos +=1;}
  if (digitalRead(buttonPin_DWN) == LOW) {y_pos -=1;}
}

void winTone() {
  for(uint8_t nLoop = 0;nLoop < 2;nLoop ++)
  {
    tone(buzzerPin,NOTE_A5);
    delay(100);
    tone(buzzerPin,NOTE_B5);
    delay(100);
    tone(buzzerPin,NOTE_C5);
    delay(100);
    tone(buzzerPin,NOTE_B5);
    delay(100);
    tone(buzzerPin,NOTE_C5);
    delay(100);
    tone(buzzerPin,NOTE_D5);
    delay(100);
    tone(buzzerPin,NOTE_C5);
    delay(100);
    tone(buzzerPin,NOTE_D5);
    delay(100);
    tone(buzzerPin,NOTE_E5);
    delay(100);
    tone(buzzerPin,NOTE_D5);
    delay(100);
    tone(buzzerPin,NOTE_E5);
    delay(100);
    tone(buzzerPin,NOTE_E5);
    delay(100);
  }
  noTone(buzzerPin);
}
