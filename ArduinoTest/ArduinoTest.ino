#include "header.h"

String serialString;
String status;

//X,Y coordinates
float xCoordinateFloat;
float yCoordinateFloat;

//Current coordinates
float last_X_coordinate = 0.000;
float last_Y_coordinate = 0.000;

int LED = 13;
bool next  = false;

void setup(){

Serial.begin(9600);
Serial.println("  _|_|_|                _|_|_|                  _|            \n_|                    _|          _|_|      _|_|_|    _|_|    \n_|  _|_|  _|_|_|_|_|  _|        _|    _|  _|    _|  _|_|_|_|  \n_|    _|              _|        _|    _|  _|    _|  _|        \n _|_|_|                _|_|_|    _|_|      _|_|_|    _|_|_| ");
pinMode(LED,OUTPUT);

}

void loop(){

  if(Serial.available()>0){

    serialString = Serial.readString();
    Serial.println(serialString[0]);

    if (serialString[0] == 'Z'){

        //Remove 1 character from the string Starting at index 0 -> Remove the Z char from the string
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

    while (next == false) {
      /* code */
      Serial.println('N');

      if (Serial.readString()[0] == 'o'){
        next = true;
        Serial.println("next = True");
      }

    }
    next = false;
  }

}
