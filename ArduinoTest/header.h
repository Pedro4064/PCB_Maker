// Include all the necessary libraries 
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Motor shield settings
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Creates all the motors' objects + servo 
Servo zServo;
Adafruit_StepperMotor *xMotor = AFMS.getStepper(200, 2);// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *yMotor = AFMS.getStepper(200, 1);// I think port #1 (M1 and M2)


// Math functions
float Modulo(float a, float b){

  if (a>0){
    a = a;
  }
  else if (a<0){
    a=a*-1;
  }

  if (b>0){
    b=b;
  }
  else if (b<0){
    b=b*-1;
  }

  return a,b;
}

//Plotting Functions
void Z(String serialString){

  float coordinate = serialString.toFloat();
  // Serial.println(coordinate);

  if (coordinate>0){
      Serial.println("It is a Z coordinate...moving servo/pen up");
      zServo.write(0); //Move servo pen up
  }

  else{
      Serial.println("It is a Z coordinate...moving servo/pen down");
      zServo.write(180); //Move servo down
  }

  // return "next";
}

float XY(String serialString){

  int stringLength = serialString.length();
  //  Serial.print("The string size is");
  //  Serial.println(serialString.length());
  int yPosition = 0;

  // The string versions of the Y and X coordinates
  String xCoordinate;
  String yCoordinate;

  // The float versions of the Y and X coordinates
  float xCoordinateFloat= 0.0;
  float yCoordinateFloat = 0.0;

  // Goes through the length of the string and find the Y position, so we can formate and devide the x from the Y coordinate
  for (int i = 0; i <= stringLength; i++) {

    if (serialString[i] == 'Y'){
        yPosition = i;
        // Serial.println("The Y positio")
    }

  }

  //Creates a substring from the original string (serialString) by deviding the string in the "Y" character
  xCoordinate = serialString.substring(1,yPosition-1);//-1 so it doesn't include the "Y" character
  yCoordinate = serialString.substring(yPosition+1,stringLength);  //+1 so it doesn't include the "Y" character

  //Add those velues to their float counterparts
  xCoordinateFloat = xCoordinate.toFloat();
  yCoordinateFloat = yCoordinate.toFloat();

  Serial.print("The x coordinate is ");
  Serial.print(xCoordinateFloat);
  Serial.println();

  Serial.print("The y coordinate is ");
  Serial.print(yCoordinateFloat);
  Serial.println();

  return xCoordinateFloat,yCoordinateFloat;
}

void MotorRun(float xCoordinateFloat, float yCoordinateFloat, float last_X_coordinate, float last_Y_coordinate){

  // Gets the difference between the current position and the desired one
  float toCoordinateX = xCoordinateFloat - last_X_coordinate;
  float toCoordinateY = yCoordinateFloat - last_Y_coordinate;

  // The modulus of the toCoordinate numbers, so we can move the steppers the right way
  float modulo_of_X,modulo_of_Y = Modulo(toCoordinateX,toCoordinateY);

  // The number of steps to move the whole sistem one milimiter -> have to check by building every thing, trial and error 
  // Example --- for one milimiter the motor needs to do 200 steps, so the numberOfStepsX = 200
  float numberOfStepsX = 0;
  float numberOfStepsY = 0;

  // Calculate the number of steps based on the numberOfStepsX and numberOfStepsY
  numberOfStepsX = numberOfStepsX*modulo_of_X;
  numberOfStepsY = numberOfStepsY*modulo_of_Y;

  // Chekcs to see if the desired coordinate is already the curret one. If so, move just the Y motor
  if (toCoordinateX == 0 ){

    Serial.println("Stay in the same x coordinate, move only the y Axis");
    //Move Y motor to position

    if (toCoordinateY < 0){
      //move counter clockwise to position
      Serial.println("Move the y motor counter clockwise to position");
    }

    else{
      //move clockwise to position
      Serial.println("Move the Y motor clockwise to position");
    }

  }

  // Chekcs to see if the desired coordinate is already the curret one. If so, move just the X motor
  else if (toCoordinateY == 0){
    Serial.println("Stay in the same Y coordinate, move only x Axis");
    //Move X Motor

    if(toCoordinateX<0){
      Serial.println("Move the X motor counter clockwise to position");

    }

    else{
      Serial.println("Move motor clock wise to position");
    }

  }

  // If both are different, move both simultaneously
  else{

    // Checks to see which of the motors will have to run the longest

    // The x is the longest, so move both until the Y arrives a location, then X continue alone
    if (modulo_of_X > modulo_of_Y){
    
      // Checks to see if both are positive values, if so, move both clockwise.
      if (toCoordinateX>0 && toCoordinateY>0){
          for (float i = 0; i = numberOfStepsY; i++){
            //Move both motors until the yMotor gets to the position, then only move the X motor
            xMotor->step(i,FORWARD,SINGLE);
            yMotor->step(i,FORWARD,SINGLE);
          }
          // move the x motor the numberOfStepsX -= numberOfStepsY -> move the x motor the rest of the steps
          xMotor->step(numberOfStepsX - numberOfStepsY,FORWARD,SINGLE);
      }

      // Checks to see if both are negative values, if so, move both counter clockwise.
      else if (toCoordinateX<0 && toCoordinateY<0){
        for (float i = 0; i = numberOfStepsY; i++)
        {
          xMotor->step(i,BACKWARD,SINGLE);
          yMotor->step(i,BACKWARD,SINGLE);
        }
          // Move the x motor the rest of the necessary steps
          xMotor->step(numberOfStepsX - numberOfStepsY,BACKWARD,SINGLE);

      }

      // Move the x clockwise and the y counter clockwise
      else if (toCoordinateX>0 && toCoordinateY<0){
        for (float i = 0; i = numberOfStepsY; i++){
          xMotor->step(i,FORWARD,SINGLE);
          yMotor->step(i,BACKWARD,SINGLE);
        }
        xMotor->step(numberOfStepsX-numberOfStepsY,FORWARD,SINGLE);
        

      }

      // move the x counter clockwise and the y clockwise
      else if (toCoordinateX<0 && toCoordinateY>0){
        for (float i = 0; i = numberOfStepsY; i++)
        {
          xMotor->step(i,BACKWARD,SINGLE);
          yMotor->step(i,FORWARD,SINGLE);
        }
        xMotor->step(numberOfStepsX-numberOfStepsY,BACKWARD,SINGLE);
        

      }

    }

    // The y is the longest, so move both until the X arrives a location, then Y continue alone
    else if (modulo_of_X < modulo_of_Y){
      
      // Checks to see if both are positive values, if so, move both clockwise.
      if (toCoordinateX>0 && toCoordinateY>0){
          for (float i = 0; i = numberOfStepsX; i++){
            //Move both motors until the yMotor gets to the position, then only move the X motor
            xMotor->step(i,FORWARD,SINGLE);
            yMotor->step(i,FORWARD,SINGLE);
          }
          // move the x motor the numberOfStepsX -= numberOfStepsY -> move the x motor the rest of the steps
          yMotor->step(numberOfStepsY - numberOfStepsX,FORWARD,SINGLE);
      }

      // Checks to see if both are negative values, if so, move both counter clockwise.
      else if (toCoordinateX<0 && toCoordinateY<0){
        for (float i = 0; i = modulo_of_Y; i++)
        {
          xMotor->step(i,BACKWARD,SINGLE);
          yMotor->step(i,BACKWARD,SINGLE);
        }
          // Move the x motor the rest of the necessary steps
          yMotor->step(numberOfStepsY - numberOfStepsX,BACKWARD,SINGLE);

      // Move the yMotor the numberOfStepsY-=numberOfStepsX
      
      }
    }

    // Both of them are equal, so move them all at once, all the way
    else if (modulo_of_X == modulo_of_Y){
      
     // Checks to see if both are positive values, if so, move both clockwise, but the same amount of steps1
     if (toCoordinateX>0 && toCoordinateY>0){
        for (float i = 0; i = numberOfStepsY; i++){
          //Move both motors until the yMotor gets to the position, then only move the X motor
          xMotor->step(i,FORWARD,SINGLE);
          yMotor->step(i,FORWARD,SINGLE);
        }
          
     }

      // Checks to see if both are negative values, if so, move both counter clockwise, but the same amount of steps
     else if (toCoordinateX<0 && toCoordinateY<0){
        for (float i = 0; i = numberOfStepsY; i++){
          xMotor->step(i,BACKWARD,SINGLE);
          yMotor->step(i,BACKWARD,SINGLE);
        }
        
     }

      // Move the x clockwise and the y counter clockwise, but the same amount of steps 
     else if (toCoordinateX>0 && toCoordinateY<0){
        for (float i = 0; i = numberOfStepsY; i++){
          xMotor->step(i,FORWARD,SINGLE);
          yMotor->step(i,BACKWARD,SINGLE);
        }
        

     }

      // move the x counter clockwise and the y clockwise, but the same amount of steps
     else if (toCoordinateX<0 && toCoordinateY>0){
        for (float i = 0; i = numberOfStepsY; i++)
        {
          xMotor->step(i,BACKWARD,SINGLE);
          yMotor->step(i,FORWARD,SINGLE);
        }
        

     }
      

    }

  }
}
