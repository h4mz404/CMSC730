/*
CMSC730 Assignment 4
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/Ouv9SU5hysM
*/

#include "pitches.h"

int sensorPin = A0;   // select the input pin for the photoresistor
int buzzerPin = 23;      // select the pin for the Buzzder
float sensorValue = 0;  // variable to store the value coming from the sensor
int randomVal;
int toneChange;
int startTime;
int currentTime;

void winTone();
void toneTwo();
void checkKeyPress();
void startGame();
void startTone();

// notes in the melody:
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  Serial.println("-----CMSC730 Assignment 4-----");
  Serial.println("-----Hamza Shah Khan-----");
  Serial.println("-----UID:119483152-----");
  Serial.println("-----Light Game-----");
  startGame();
}

void loop() {
  
  currentTime = millis();
  if(currentTime - startTime <10000){

    tone(buzzerPin, melody[0]-toneChange, 250);
    int pauseBetweenNotes = 250 * 1.0;// to distinguish the notes, set a minimum time between them. the note's duration + 30% seems to work well:
    delay(pauseBetweenNotes);
    noTone(buzzerPin);    // stop the tone playing:

    //Serial.print("Random Value: "+ String(randomVal));
    sensorValue = analogRead(sensorPin);
    //Serial.println("\tSensor Value: " + String(sensorValue));
    int isClose = randomVal - sensorValue;
    //Serial.print("\t Change:  " + String(isClose));

    if (abs(isClose)<150 && abs(isClose)>-150){ //Margin of +/-150
      for(int i=0;i<5;i++){
        analogWrite(buzzerPin,255);
        delay(100);
        analogWrite(buzzerPin,0);
        delay(100);}
      Serial.print("Target Value: "+ String(randomVal));
      Serial.println("\tSensor Value: " + String(sensorValue));
      Serial.println("\n\n***************YOU WIN**************\n");
      winTone();
      delay(1000);
      checkKeyPress();
      startGame();
      }
    else{
    toneChange = constrain(map(isClose,-2600,2600,-100,100),-100,100);
    //Serial.print("\t ToneChange:  " + String(toneChange) + "\n");
    delay(10);
    }
  }
  else{
    Serial.println("\n\n***************YOU LOSE**************\n");
    toneTwo();
    delay(1000);
    checkKeyPress();
    startGame();
  }
}

void winTone() {
  int melodyWin[] = { //tone array
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0,
    NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_A3, NOTE_G3,
    NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_A3, NOTE_G3,
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3, 0};
  int noteDurationsWin[] = {4, 8, 8, 4, 4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 4};
  for (int thisNote = 0; thisNote < 23; thisNote++) {
    int noteDuration = 1000 / noteDurationsWin[thisNote];
    tone(buzzerPin, melodyWin[thisNote], noteDuration); //play tone
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);//delay
    noTone(buzzerPin);//tone off
  }
}

void toneTwo() {
  int melody2[] = {NOTE_GS7, NOTE_DS8, NOTE_GS7, 0, NOTE_DS8, NOTE_DS8, 0, NOTE_GS7, NOTE_GS7};
  int noteDurations[] = {4, 8, 8, 4, 8, 8, 4, 4, 4};
  for (int thisNote = 0; thisNote < 9; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody2[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void checkKeyPress(){
  Serial.println("Press 'r' to reset game");
  while(Serial.available()==0 or Serial.available()!=0){
    char command = Serial.read();
    if (command == 'r') break;
  }
}

void startTone(){
  int melody2[] = {NOTE_E6, NOTE_G6, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_G7};
  int noteDurations[] = {8, 8, 8, 8, 8, 8};
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody2[thisNote], noteDuration);
    delay(130);
    noTone(buzzerPin);
  }
  noTone(buzzerPin);
}

void startGame(){
  startTone();
  delay(100);
  randomVal = random(2000);
  startTime = millis();
  Serial.println("\n\n***************GAME START**************\n");
}
