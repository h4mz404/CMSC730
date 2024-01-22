/*
CMSC730 Assignment 6
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/ljd9-L3WObU
*/

#include <Wire.h>

const int sda = 21;
const int scl = 22;
float accX,accY,accZ;
float gX,gY,gZ;
int marker = 1;
byte ACCEL_OUT_H = 0;
byte ACCEL_OUT_L = 0;
byte GYRO_OUT_H = 0;
byte GYRO_OUT_L = 0;
int16_t ACCEL_RAW = 0;
int16_t GYRO_RAW = 0;
const int MPU = 0x68;

void setup() {
Serial.begin(9600);
Wire.begin(sda, scl); //SDA, SCL
  Serial.println("-----CMSC730 Assignment 6-----");
  Serial.println("-----Hamza Shah Khan-----");
  Serial.println("-----UID:119483152-----");
  Serial.println("-----IMU + I2C-----");
  Serial.println("-----Press 'A' for Accelerometer Data-----");
  Serial.println("-----Press 'G' for Gyroscope Raw Data-----");
}

void loop() {

  accX = getData(0x3B); //Register Addresses
  accY = getData(0x3D);
  accZ = getData(0x3F);
  gX   = getData(0x43);
  gY   = getData(0x45);
  gZ   = getData(0x47);

  if (Serial.available() > 0) {
    char command = Serial.read(); 
    if (command == 'A') marker = 1; //Set marker to 1 for Accelerometer Values
    else if (command == 'G') marker = 0; //Set marker to 0 for Gyroscope Values
    else marker = marker; //If no input, let marker be set to previous input
  }

  if (marker) {
      Serial.println("Accelerometer Data (X,Y,Z) (g):  ");
      Serial.print(accX); Serial.print(", "); Serial.print(accY); Serial.print(", ");Serial.println(accZ);
      delay(50);} 
  else {
      Serial.println("Gyro Raw Data (X,Y,Z): ");
      Serial.print(gX); Serial.print(", "); Serial.print(gY); Serial.print(", ");Serial.println(gZ);
      delay(50);}
}

//Function to read Accelerometer Data
float getData(int address){
  Wire.beginTransmission(MPU);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 2);
  ACCEL_OUT_H = Wire.read();
  ACCEL_OUT_L = Wire.read();
  ACCEL_RAW = ACCEL_OUT_H << 8 | ACCEL_OUT_L;
  return ACCEL_RAW / 16384.0; // Returns value in g(m/s^2)
}

//Function to read Gyro Data
float getGyroData(int address){
  Wire.beginTransmission(MPU);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 2);
  GYRO_OUT_H = Wire.read();
  GYRO_OUT_L = Wire.read();
  GYRO_RAW = GYRO_OUT_H << 8 | GYRO_OUT_L;
  return GYRO_RAW;  //Returns raw gyro data
}

