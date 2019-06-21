#include "header.h"

// Encoder knob(1,2,3);
String Potato;

void setup(){
Serial.begin(9600);

}

void loop(){

  // knob.check();
  if(Serial.available()>0){

    Potato = Serial.readString();
    Potato.remove(0,1);
    Serial.println(Potato);
  }

}
