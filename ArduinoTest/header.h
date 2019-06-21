
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
