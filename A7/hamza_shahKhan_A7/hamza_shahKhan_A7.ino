/*
CMSC730 Assignment 7
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/Gu1AB9mekLY
*/

#include <Wire.h>
const int sda = 21;
const int scl = 22;

int16_t GYRO_X_RAW = 0;
int16_t GYRO_Y_RAW = 0;
int16_t GYRO_Z_RAW = 0;

float accX,accY,accZ;
float gX,gY,gZ;
float gyroAngleX = 0, gyroAngleY = 0, gyroAngleZ = 0;
float currentTime = 0;
float previousTime = 0;
float elapsedTime = 0;
float caliX = 0, caliY = 0, caliZ = 0;

const int MPU = 0x68;

void setup() {
  Serial.begin(115200);
  Wire.begin(sda, scl); //SDA, SCL
  calibrateGyro();
}


void loop() {
  currentTime = millis();
  elapsedTime = currentTime - previousTime;
  previousTime = currentTime;
  readGyro();
  processGyro();
  Serial.print(gyroAngleX);
  Serial.print(", ");
  Serial.print(gyroAngleY);
  Serial.print(", ");
  Serial.println(gyroAngleZ);
  delay(50);
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

void readAcc(){
  Wire.beginTransmission(MPU);
  Wire.write(0X3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 6);
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
}
