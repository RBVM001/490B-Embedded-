// This example code is in the public domain.

import processing.serial.*;
import processing.opengl.*;


int bgcolor;                 // Background color
int fgcolor;                 // Fill color
Serial myPort;                       // The serial port
int[] serialInArray = new int[225];    // Where we'll put what we receive
int[] pastInArray = new int [225];
float[][] colorTarget   = new float[3][255];
float[][] currentColor   = new float[3][255];
PVector[][] vertices = new PVector[15][15];
float[] verticesTZ = new float[15];
float w = 30;
float ease = 0.75; 

int serialCount = 0;                 // A count of how many bytes we receive
int xpos, ypos;                  // Starting position of the ball
boolean firstContact = false;        // Whether we've heard from the microcontroller
int tiempoant;
int render=0;
int dif=0;

void setup() {
  size(960, 600, OPENGL);  // Stage size
  noStroke();      // No border on the next thing draw
  
  
  // Print a list of the serial ports, for debugging purposes:
  println(Serial.list());

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  
  myPort = new Serial(this, Serial.list()[2], 115200);
  
  for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 15; i++) {
            vertices[i][j] = new PVector( i*w, j*w, 0);
        }
    }
    
  
}

void draw() {

  if (render==1) {
    
    translate(width/4, 100);
    rotateX(0.5);
    //rotateX(PI/10);
    background(0);
    for (int j=0; j<14; j++) {
      beginShape(QUAD_STRIP);
      for (int i=0; i<15; i++) {
          stroke(255);
     
          fill(serialInArray[j*15+i], 0, 0);
          float x = i*width/15;
          float y = j*height/15;
          verticesTZ[i] = serialInArray[j*15+i];
          
          vertices[i][j].z += (verticesTZ[i]-vertices[i][j].z)*ease; //value that causes z axis to increase
         // if(vertices[i][j].z > 200)
         // println(vertices[i][j].z);
          vertex( vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
          vertex( vertices[i][j+1].x, vertices[i][j+1].y, vertices[i][j+1].z);
        }
         endShape(CLOSE);
        //        println();
      }
      render=0;
  }
}

void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A,
  // clear the serial buffer and note that you've
  // h
  //  ad first contact from the microcontroller. 
  // Otherwise, add the incoming byte to the array:
  if (firstContact == false) {
    if (inByte == 'A') { 
      myPort.clear();          // clear the serial port buffer
      firstContact = true;     // you've had first contact from the microcontroller
      myPort.write('A');       // ask for more
    }
  } else {
    // Add the latest byte from the serial port to array:

      serialInArray[serialCount] = inByte;

    serialCount++;

    // If we have 3 bytes:
    if (serialCount > 224 ) {
      if (millis()-tiempoant > 50 && millis() - tiempoant <40)
     println(millis()-tiempoant);
      tiempoant = millis();
      
      render = 1;
    
      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}
