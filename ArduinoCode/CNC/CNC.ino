#include "functions.h"

// Motor shield settings
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Creates the servo instace and get the pointers to the steppers
Servo zServo;
Adafruit_StepperMotor* xMotor = AFMS.getStepper(2038, 2);// to motor port #2 (M3 and M4)
Adafruit_StepperMotor* yMotor = AFMS.getStepper(513, 1);// I think port #1 (M1 and M2)

void setup()
{

    // Start serial communication, boundrate 9600
    Serial.begin(9600);
    Serial.println("  _|_|_|                _|_|_|                  _|            \n_|                    _|          _|_|      _|_|_|    _|_|    \n_|  _|_|  _|_|_|_|_|  _|        _|    _|  _|    _|  _|_|_|_|  \n_|    _|              _|        _|    _|  _|    _|  _|        \n _|_|_|                _|_|_|    _|_|      _|_|_|    _|_|_| ");

    // Set the servo pin to 53 and set to home position
    zServo.attach(53);
    zServo.write(0);

    // create with the default frequency 1.6KHz
    AFMS.begin();
}

void loop()
{
    // Instantiate 2 structs, one for the cirrent point and one to the newxt point 
    coordinates last_coord;
    coordinates next_coord;

    // The struct that will hold the speed of the steppers
    speed steppers_speeds;

    // If the serial port is open
    if (Serial.available() > 0)
    {
        String data = Serial.readString();

        // Check to see if the data refers to the z position of the x/y position
        switch (data[0])
        {
            case 'Z':

                Serial.println("Moving Servo up");

                // parse the data and move the servo 
                update_z(data, &zServo);
                break;
    
            case 'X':

                Serial.println("Moving Servo down");

                // Update the data in the structs
                update_xy(data, &next_coord);

                // calculate the speeds for each stepper and save them to the struct
                calculate_speed(&next_coord, &last_coord, &steppers_speeds, 255);

                // Move the steppers 
                move_steppers( xMotor, yMotor, &steppers_speeds, &last_coord, &next_coord);
                break;
            
            default:

                Serial.println("Junk data on Serial");
                break;
        }
    }

    // update the last_coord values
    last_coord = next_coord;

}
