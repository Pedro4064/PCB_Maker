#include "header.h"

// Encoder knob(1,2,3);
String serialString;
String status;

//X,Y coordinates
float xCoordinateFloat;
float yCoordinateFloat;

int LED = 13;

void setup(){

Serial.begin(9600);
pinMode(LED,OUTPUT);

}

void loop(){

  // knob.check();
  if(Serial.available()>0){

    serialString = Serial.readString();

    Serial.println(serialString[0]);
    if (serialString[0] == 'Z'){

        serialString.remove(0,1);
        Z(serialString);

        // Sends a signal to the raspberry pi to tell it ot send the next line
        digitalWrite(LED,HIGH);
        delay(10);
        digitalWrite(LED,LOW);


    }

    else if (serialString[0] == 'X'){

        xCoordinateFloat,yCoordinateFloat = XY(serialString);

        // Sends a signal to the raspberry pi to tell it ot send the next line
        digitalWrite(LED,HIGH);
        delay(10);
        digitalWrite(LED,LOW);



    }
    
  }

}
