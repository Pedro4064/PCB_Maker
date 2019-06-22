#include "header.h"

// Encoder knob(1,2,3);
String serialString;
String status;

//X,Y coordinates
float xCoordinateFloat;
float yCoordinateFloat;

//Current coordinates
float last_X_coordinate = 0.000;
float last_Y_coordinate = 0.000;

int LED = 13;

void setup(){

Serial.begin(9600);
Serial.println("  _|_|_|                _|_|_|                  _|            \n_|                    _|          _|_|      _|_|_|    _|_|    \n_|  _|_|  _|_|_|_|_|  _|        _|    _|  _|    _|  _|_|_|_|  \n_|    _|              _|        _|    _|  _|    _|  _|        \n _|_|_|                _|_|_|    _|_|      _|_|_|    _|_|_| ");
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

        //Parses the serial communication and returns the x and y coordinates
        xCoordinateFloat,yCoordinateFloat = XY(serialString);

        //Uses the coordinates to move the stepper motors
        MotorRun(xCoordinateFloat,yCoordinateFloat,last_X_coordinate,last_Y_coordinate);

        //saves the coordinates as the last one for the next round
        last_X_coordinate = xCoordinateFloat;
        last_Y_coordinate = yCoordinateFloat;

        // Sends a signal to the raspberry pi to tell it ot send the next line
        digitalWrite(LED,HIGH);
        delay(10);
        digitalWrite(LED,LOW);



    }

  }

}
