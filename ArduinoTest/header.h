
class Encoder {

  private:
    int plusPin    = 0;
    int minusPin   = 0;
    int buttonPin  = 0;
    int number     = 0;
    bool buttonPressed = false;

  public:
    Encoder (int plus, int minus, int button);
    int check();
    bool checkButtonPress();
    void zeroCounter();

};

Encoder::Encoder(int plus, int minus, int button){
  plusPin = plus;
  minusPin = minus;
  buttonPin = button;

  pinMode(plusPin,INPUT);
  pinMode(minusPin,INPUT);
  pinMode(buttonPin,INPUT);

}

int Encoder::check(){

  // Checks if the encoder was turned clockwise(+) or counter clockwise(-)
  if (digitalRead (plusPin) == HIGH){

      number++;
      return number;
  }
  else if (digitalRead(minusPin) == HIGH){

      number--;
      return number;
  }

}

bool Encoder::checkButtonPress(){

  if (digitalRead(buttonPin) == HIGH){
    return true;
  }
  else{
    return false;
  }
}

void Encoder::zeroCounter(){

  number = 0;
}



//Plotting Functions

void Z(String serialString){

  float coordinate = serialString.toFloat();
  Serial.println(coordinate);

  if (coordinate>0){
      Serial.println("It is a Z coordinate...moving servo/pen up");
  }

  else{
      Serial.println("It is a Z coordinate...moving servo/pen down");
  }

  // return "next";
}

float XY(String serialString){

  int stringLength = serialString.length();
  int yPosition = 0;

  // The string versions of the Y and X coordinates
  String xCoordinate;
  String yCoordinate;

  // The float versions of the Y and X coordinates
  float xCoordinateFloat= 0.0;
  float yCoordinateFloat = 0.0;

  // Goes through the length of the string and find the Y position, so we can formate and devide the x from the Y coordinate
  for (int i = 0; i <= stringLength; i++) {

    if (serialString[i] == "Y"){
        yPosition = i;
        // Serial.println("The Y positio")
    }

  }

  //Creates a substring from the original string (serialString) by deviding the string in the "Y" character
  xCoordinate = serialString.substring(1,yPosition-1);//-1 so it doesn't include the "Y" character
  yCoordinate = serialString.substring(yPosition+1);  //+1 so it doesn't include the "Y" character

  //Add those velues to their float counterparts
  xCoordinateFloat = xCoordinate.toFloat();
  yCoordinateFloat = yCoordinate.toFloat();

  Serial.print("The x coordinate is ");
  Serial.print(xCoordinateFloat);
  Serial.println();

  Serial.println("The y coordinate is ");
  Serial.println(yCoordinateFloat);

  return xCoordinateFloat,yCoordinateFloat;
}
