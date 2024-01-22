/*
CMSC730 Assignment 8
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/3sDYnCJoyuI
*/

#include <Wire.h>
const int sda = 21;
const int scl = 22;

int16_t GYRO_X_RAW = 0;
int16_t GYRO_Y_RAW = 0;
int16_t GYRO_Z_RAW = 0;

float gX,gY,gZ;
float gyroAngleX = 0, gyroAngleY = 0, gyroAngleZ = 0;
float currentTime = 0;
float previousTime = 0;
float elapsedTime = 0;
float caliX = 0, caliY = 0, caliZ = 0;

const int MPU = 0x68;

int latchPin = 18; // Latch pin of 74HC595 
int clockPin = 5; // Clock pin of 74HC595 
int dataPin = 19; // Data pin of 74HC595 

byte led_data = 0;

int prev_data = 0;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  pinMode(clockPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("-----CMSC730 Assignment 8-----");
  Serial.println("-----Hamza Shah Khan-----");
  Serial.println("-----UID:119483152-----");
  Serial.println("-----Tilt Indicator-----");
  Wire.begin(sda, scl); //SDA, SCL
  calibrateGyro();
  updateShiftRegister();
}


void loop() {
  currentTime = millis();
  elapsedTime = currentTime - previousTime;
  previousTime = currentTime;
  readGyro();
  processGyro();
  delay(50);
  int led = constrain(map(gyroAngleX,-90,90,-8,8),-8,8);
  // Serial.println("Gyro Mapped");
  Serial.println(led);
  // Serial.println("BYTE");
  // Serial.println(led_data);
  //led_data = byte(led);
  updateShiftRegister();
  if (led<0){
    led_data = 0;
    bitSet(led_data,abs(led)-1);
  }
  else if(led>0){
    led_data = 0;
    bitSet(led_data,abs(abs(led)-8));
  }
  else if(led == 0){
    led_data = 0;
  }
  updateShiftRegister();
}


void readGyro(){
  //Serial.println("Readin gyro Data");
  Wire.beginTransmission(MPU);
  Wire.write(0X43);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 6);
  GYRO_X_RAW = Wire.read() << 8 | Wire.read();
  GYRO_Y_RAW = Wire.read() << 8 | Wire.read();
  GYRO_Z_RAW = Wire.read() << 8 | Wire.read();
  gX = GYRO_X_RAW/131.0;
  gY = GYRO_Y_RAW/131.0;
  gZ = GYRO_Z_RAW/131.0;
}

void processGyro(){
  //Serial.println("Processing gyro Data");
  elapsedTime = elapsedTime/1000;
  gX = gX - caliX;
  gY = gY - caliY;
  gZ = gZ - caliZ;
  gyroAngleX = gyroAngleX + gX * elapsedTime;
  gyroAngleY = gyroAngleY + gY * elapsedTime;
  gyroAngleZ = gyroAngleZ + gZ * elapsedTime;

}

void calibrateGyro(){
  Serial.println("Calibrating Gyroscope Offset Values...........");
  int calib = 200;
  for(int i = 0;i<calib;i++){
    Wire.beginTransmission(MPU);
    Wire.write(0X43);
    Wire.endTransmission();
    Wire.requestFrom(MPU, 6);
    GYRO_X_RAW = Wire.read() << 8 | Wire.read();
    GYRO_Y_RAW = Wire.read() << 8 | Wire.read();
    GYRO_Z_RAW = Wire.read() << 8 | Wire.read();
    gX = GYRO_X_RAW/131.0;
    gY = GYRO_Y_RAW/131.0;
    gZ = GYRO_Z_RAW/131.0;
    caliX += gX;
    caliY += gY;
    caliZ += gZ;
    delay(20);
  }
  caliX = caliX/calib;
  caliY = caliY/calib;
  caliZ = caliZ/calib;
  Serial.print("Calibration done, X,Y,Z Offsets: ");
  Serial.print(caliX);
  Serial.print(", ");
  Serial.print(caliY);
  Serial.print(", ");
  Serial.println(caliZ);
}


void updateShiftRegister()
{
 digitalWrite(latchPin, LOW);
 shiftOut(dataPin, clockPin, LSBFIRST, led_data);
 digitalWrite(latchPin, HIGH);
}