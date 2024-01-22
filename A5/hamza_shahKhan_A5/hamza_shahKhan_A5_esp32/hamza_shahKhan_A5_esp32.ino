/*
CMSC730 Assignment 5
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/Cewr4ZVRQzU
*/

//Input and Output Pins
int servoPin = 13;
int trigPin = 23;
int echoPin = 22;
int delayTime = 15;
int distance = 0;

//Function Declarations
int dist();
void angle(int a);

void setup() {
  Serial.begin(9600);
  pinMode(servoPin,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);
}

void loop() {
  for(int pos=0;pos<180;pos++){
    dist();
    angle(pos);
    delay(delayTime);
    Serial.print(pos);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
  for(int pos=180;pos>0;pos--){
    dist();
    angle(pos);
    delay(delayTime);
    Serial.print(pos);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    }
  }

//Converts angular position into appropriate PWM signal
void angle(int a){
    int highTime = map(a,0,180,500,2600); //Converting angular position to duty cycle
    int lowTime = 20000 - highTime;  //20ms Time Period
    digitalWrite(servoPin,HIGH);
    delayMicroseconds(highTime);
    digitalWrite(servoPin,LOW);
    delayMicroseconds(lowTime);
}

//Returns Distance from Ultrasonic Sensor
int dist(){
  long duration = -1;
  distance = -1;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)*0.0343;
  // if(distance>=100) Serial.println("Out of Range");
  // else{
  //   Serial.print(distance);
  //   Serial.println("cm");
  // }
  return distance;
}