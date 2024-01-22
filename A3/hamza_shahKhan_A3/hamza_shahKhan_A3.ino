/*
\n
CMSC730 Assignment 3
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
*/
#include <iostream>

const int dot = 100;
const int dash = 3*dot;
const int led = 23;

int alphabets[26][4]={ {dot, dash},{dash, dot, dot, dot},{dash, dot, dash, dot},{dash, dot, dot},{dot},{dot, dot, dash, dot},{dash, dash, dot},//A-G
{dot, dot, dot, dot},{dot, dot},{dot, dash, dash, dash},{dash, dot, dash},{dot, dash, dot, dot},{dash, dash},{dash, dot},{dash, dash, dash},//H-O
{dot, dash, dash, dot},{dash, dash, dot, dash},{dot, dash, dot},{dot, dot, dot},{dash},{dot, dot, dash},{dot, dot, dot, dash},{dot, dash, dash}, //P-W
{dash, dot, dot, dash},{dash, dot, dash, dash},{dash, dash, dot, dot}};

int numbers[10][5]= {{dash, dash, dash, dash, dash},{dot, dash, dash, dash, dash},{dot, dot, dash, dash, dash},{dot, dot, dot, dash, dash},
{dot, dot, dot, dot, dash},{dot, dot, dot, dot, dot},{dash, dot, dot, dot, dot},{dash, dash, dot, dot, dot},{dash, dash, dash, dot, dot},{dash, dash, dash, dash, dot}}; //0-9


void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Serial.println("-----CMSC730 Assignment 3-----");
  Serial.println("-----Hamza Shah Khan-----");
  Serial.println("-----UID:119483152-----");
  Serial.println("-----Morse Code Converter-----");
}


void loop(){

  Serial.println("\n\nEnter text to be converted to Morse: ");

  String input;
  while(Serial.available()==0){}
  input = Serial.readString();
  Serial.print("Converting to Morse Code: "+ String(input) );
  Serial.print("\nMorse Code: ");
  for (char x: input){
    if (x >= 'A' && x<='Z'){
      letter2Morse(alphabets[x - 'A']);
    }
    else if (x>='a' && x<='z'){
      letter2Morse(alphabets[x - 'a']);
    }
    else if (x>='0' && x<='9')
      number2Morse(numbers[x-'0']);
    else if(x==' ')
      delay(3*dot);
  }

}

void letter2Morse(int* letter){
    for (int i{0};i<=3;i++){
    //std::cout << alphabets['C'-'A'][i] << '\n';
        if (letter[i]!=0){
          if(letter[i]==dot) Serial.print("dot ");
          else Serial.print("dash ");
          flashMorse(letter[i]);}
    }
}

void number2Morse(int* letter){
    for (int i{0};i<=4;i++){
    //std::cout << alphabets['C'-'A'][i] << '\n';
        if (letter[i]!=0){
          if(letter[i]==dot) Serial.print("dot ");
          else Serial.print("dash ");
          flashMorse(letter[i]);}
    }
}

void flashMorse(int delay_time){
    digitalWrite(led,HIGH);
    delay(delay_time);
    digitalWrite(led,LOW);
    delay(dot);
}



