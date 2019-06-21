#include "header.h"

// Encoder knob(1,2,3);
String serialString;
String status;

//X,Y coordinates
float xCoordinateFloat;
float yCoordinateFloat;


void setup(){
Serial.begin(9600);

}

void loop(){

  // knob.check();
  if(Serial.available()>0){

    serialString = Serial.readString();

    Serial.println(serialString[0]);
    if (serialString[0] == 'Z'){

        serialString.remove(0,1);
        Z(serialString);

    }

    else if (serialString[0] == "X"){

        xCoordinateFloat,yCoordinateFloat = XY(serialString);

    }
    // serialString.remove(0,1);
    // Serial.println(serialString);
  }

}
