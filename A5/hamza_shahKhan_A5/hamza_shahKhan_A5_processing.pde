/*
CMSC730 Assignment 5
University of Maryland, College Park
Fall 2023
Author: Hamza Shah Khan
UID: 119483152
Directory ID: hamzask@umd.edu
Link for output video: https://youtu.be/Cewr4ZVRQzU
*/

// imports libraries for serial communication
import processing.serial.*; 
import java.awt.event.KeyEvent; 
import java.io.IOException;

Serial myPort;
String angle="";
String distance="";
String data="";
int index1=0;
int height = 700;
int width = 330;
int inputAngle;
int inputDist;

void setup(){
//Setup Radar Screen Dimensions
size(700,400);
smooth();
background(0,0,0);                       //Set background to black
// starts the serial communication
myPort = new Serial(this,"COM3", 9600);  
myPort.bufferUntil('.');
}

//Main Loop
void draw() {
  //To fade the previous scan
  noStroke();
  fill(0,0,0,4); 
  rect(0, 0, 700, 400); 
  //Calling functions to display radar output
  drawRadar();
  drawLine();
  drawDetectedObject();
  drawOutput();
}

//Get Angle and Distance values from Serial port
void serialEvent(Serial myPort){
  data = myPort.readStringUntil('.');
  data = data.substring(0,data.length()-1);
  index1 = data.indexOf(","); // find the character ',' and puts it into the variable "index1"
  angle= data.substring(0, index1); // read the data from position "0" to position of the variable index1 or thats the value of the angle the Arduino Board sent into the Serial Port
  distance= data.substring(index1+1, data.length()); // read the data from position "index1" to the end of the data pr thats the value of the distance
  inputAngle = int(map(int(angle),0,180,180,0));
  inputDist = int(distance);
}

//Draws the main GUI for the RADAR
void drawRadar(){
translate(350,360); 
strokeWeight(2);
stroke(98,245,31); 
noFill();
arc(0, 0, 600, 600, PI, TWO_PI);
arc(0, 0, 450, 450, PI, TWO_PI);
arc(0, 0, 300, 300, PI, TWO_PI);
arc(0, 0, 150, 150, PI, TWO_PI);
line(-width,0,width,0);
line(0,0,-width*cos(radians(30)),-width*sin(radians(30)));
line(0,0,-width*cos(radians(60)),-width*sin(radians(60)));
line(0,0,-width*cos(radians(90)),-width*sin(radians(90)));
line(0,0,-width*cos(radians(120)),-width*sin(radians(120)));
line(0,0,-width*cos(radians(150)),-width*sin(radians(150)));
textSize(13);
//To remove faded text
fill(0,0,0);
noStroke();
rect(-width, 0, width*2, 40);
fill(98,245,31);
text("10cm",45,-2);
text("20cm",120,-2);
text("30cm",195,-2);
text("40cm",270,-2);
text("30°",-25-width*cos(radians(30)),-width*sin(radians(30)));
text("60°",-20-width*cos(radians(60)),-10-width*sin(radians(60)));
text("90°",-10-width*cos(radians(90)),-10-width*sin(radians(90)));
text("120°",5-width*cos(radians(120)),-10-width*sin(radians(120)));
text("150°",10-width*cos(radians(150)),-width*sin(radians(150)));
}

//Draws the scanning line according to current angle of the servo
void drawLine(){
  strokeWeight(4);
  stroke(98,245,0); 
  line(0,0,-width*cos(radians(inputAngle)),-width*sin(radians(inputAngle)));
}

//Draws red line when object in range, visualizing how far the object is
void drawDetectedObject(){
  strokeWeight(4);
  stroke(255,10,10); 
  float pixelDist = inputDist * 7.5;
  if (inputDist<40)
    line(-pixelDist*cos(radians(inputAngle)),-pixelDist*sin(radians(inputAngle)),-width*cos(radians(inputAngle)),-width*sin(radians(inputAngle)));
}

//Draws/Outputs the Serial Data from the ESP32
void drawOutput(){
  textSize(20);
  fill(98,245,31);
  if (inputDist>40){
  text("Object: Out of Range",-300,26); 
  text("Angle: ",100,26);
  text("Distance: ",200,26);
  }
  else{
  text("Object: In Range",-300,26);   `
  text("Angle: " + inputAngle,100,26);
  text("Distance: " + inputDist,200,26);
  }
}
