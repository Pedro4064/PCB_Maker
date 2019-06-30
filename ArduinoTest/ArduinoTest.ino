#include "header.h"

String serialString;
String status;

//X,Y coordinates
float xCoordinate_Float;
float yCoordinate_Float;

//Current coordinates
float lastX_coordinate = 0.000;
float lastY_coordinate = 0.000;

int LED = 13;
bool next  = false;

void setup(){

  Serial.begin(9600);
  Serial.println("  _|_|_|                _|_|_|                  _|            \n_|                    _|          _|_|      _|_|_|    _|_|    \n_|  _|_|  _|_|_|_|_|  _|        _|    _|  _|    _|  _|_|_|_|  \n_|    _|              _|        _|    _|  _|    _|  _|        \n _|_|_|                _|_|_|    _|_|      _|_|_|    _|_|_| ");

  zServo.attach(20);//Attaches the servo ro pin 20 on the arduino Mega and move it to position 0
  zServo.write(0);

  xMotor->setSpeed(10); //10 rpm, we need to test and see if it is enough
  yMotor->setSpeed(10);

  pinMode(LED,OUTPUT);

}

void loop() {

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
        xCoordinate_Float,yCoordinate_Float = XY(serialString);

        //Uses the coordinates to move the stepper motors
        MotorRun(xCoordinate_Float,yCoordinate_Float,lastX_coordinate,lastY_coordinate);

        //saves the coordinates as the last one for the next round
        lastX_coordinate = xCoordinate_Float;
        lastY_coordinate = yCoordinate_Float;

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
