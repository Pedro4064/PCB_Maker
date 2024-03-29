#include "header.h"


void setup(){

  Serial.begin(9600);
  Serial.println("  _|_|_|                _|_|_|                  _|            \n_|                    _|          _|_|      _|_|_|    _|_|    \n_|  _|_|  _|_|_|_|_|  _|        _|    _|  _|    _|  _|_|_|_|  \n_|    _|              _|        _|    _|  _|    _|  _|        \n _|_|_|                _|_|_|    _|_|      _|_|_|    _|_|_| ");

  zServo.attach(53);//Attaches the servo ro pin 20 on the arduino Mega and move it to position 0
  zServo.write(0);

  AFMS.begin();  // create with the default frequency 1.6KHz
  xMotor->setSpeed(20); //10 rpm, we need to test and see if it is enough
  yMotor->setSpeed(20);

  

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
        XY(serialString);

        //Uses the coordinates to move the stepper motors
        MotorRun(xCoordinate_Float,yCoordinate_Float,lastX_coordinate,lastY_coordinate);

        //saves the coordinates as the last one for the next round
        lastX_coordinate = xCoordinate_Float;
        lastY_coordinate = yCoordinate_Float;

        Serial.println(lastX_coordinate);
        Serial.println(lastY_coordinate);

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
