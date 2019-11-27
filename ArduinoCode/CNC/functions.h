#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>


// The struct that wull hold the coordinates for the points
struct coordinates
{
    float x = 0;
    float y = 0;
};

struct speed
{
    float xSpeed;
    float ySpeed;
};

inline float module(float a)
{
    if (a<0)
    {
        return a*-1;
    }
    else
    {
        return a;
    }
    
}
void update_z(String raw_data, Servo* servo)
{
    // parse the data and turn it to a float
    raw_data.remove(0,1);  //Remove 1 character from the string Starting at index 0 -> Remove the Z char from the string
    float z_position = raw_data.toFloat();

    // If the z position is bigger than 0, move up the tip, off the bed
    if (z_position > 0)
    {
        // move servo up
        servo->write(0);
    }

    else
    {
        // move servo down
        servo->write(180);
    }
    
}

void update_xy(String raw_data, coordinates* new_coord)
{
    // Get the length of the string
    int string_size = raw_data.length();

    // The position of the y character, it will be used to split and format the data
    int yChar_position;

    // Loop through the string and save the Y character index
    for (int i; i <= string_size; i++)
    {
        if (raw_data[i] == 'Y')
        {
            yChar_position = i;
        }
    }

    // The string versions of the Y and X coordinates
    String xCoordinate;
    String yCoordinate;

    //Creates a substring from the original string (serialString) by deviding the string in the "Y" character
    xCoordinate = raw_data.substring(1,yChar_position-1);             //-1 so it doesn't include the "Y" character
    yCoordinate = raw_data.substring(yChar_position+1,string_size);  //+1 so it doesn't include the "Y" character

    // update the data in the struct
    new_coord->x = xCoordinate.toFloat();
    new_coord->y = yCoordinate.toFloat();

}

inline float proportional_speed(float biggest_distance, float small_distance, float max_speed)
{
    /*
    biggest_distance -------- max_speed
    small_distance ---------- proportional_speed
    */
   float speed = (small_distance*max_speed)/biggest_distance;
   return speed;
}

void calculate_speed(coordinates* next_coordinate, coordinates* current_coordinate, speed* stepper_speeds, float max_speed)
{
    // See which stepper will have to move the most 
    float dX = next_coordinate->x - current_coordinate->x;
    float dY = next_coordinate->y - current_coordinate->y;

    // Get the module of the distance
    dX = module(dX);
    dY = module(dY);

    if (dX>dY)
    {
        // If the x axis stepper will need to mave more, it will have the highest rpm possible
        stepper_speeds->xSpeed = max_speed;
        stepper_speeds->ySpeed = proportional_speed(dX,dY,max_speed);
    }

    else
    {
        stepper_speeds->ySpeed = max_speed;
        stepper_speeds->xSpeed = proportional_speed(dY,dX,max_speed);
    }
    
}

void move_steppers(Adafruit_StepperMotor* x_stepper, Adafruit_StepperMotor* y_stepper, speed* steppers_speed, coordinates* current_coordinates, coordinates* next_coordinates)
{
    // number of steps to move one millimiter
    float step_length = 10; //example number, needs to see the gear ration on the belt

    float x_steps = (next_coordinates->x - current_coordinates->x) * step_length;
    float y_steps = (next_coordinates->y - current_coordinates->y) * step_length;

    // Set the speed 
    x_stepper->setSpeed(steppers_speed->xSpeed);
    y_stepper->setSpeed(steppers_speed->ySpeed);

    // The direction of the steppers
    uint8_t y_direction;
    uint8_t x_direction;

    // If the number of steps is negative, the direction is BACKWARD, else the direction is FORWARD
    if (y_steps < 0)
    {
        y_direction = BACKWARD;
    }
    else
    {
        y_direction = FORWARD;
    }
    

    if (x_steps < 0)
    {
        x_direction = BACKWARD;
    }
    else
    {
        x_direction = FORWARD;
    }

    
    if (module(x_steps) > module(y_steps))
    {
        // Get the "slope", basically the ratio of x and y ---- Δx/Δy
        float ratio = x_steps/y_steps;
        
        // command each step 
        for (int step = 0; step<module(y_steps); step++)
        {
            // Move the y_stepper
            y_stepper->step(1, y_direction, SINGLE);

            // move the x stepper -- it will move more, but faster so the time it takes should be the same as the y stepper
            x_stepper->step(ratio, x_direction, SINGLE);

        }
    }

    else
    {
        // Get the "slope", basically the ratio of y and x ---- Δy/Δx
        float ratio = y_steps/x_steps;

        // Command each step
        for (int step = 0; step<module(x_steps); step++)
        {
            // Move the x stepper
            x_stepper->step(1,x_direction, SINGLE);

            // move the y stepper -- it will move more, but faster so the time it takes should be the same as the y stepper
            y_stepper->step(ratio, y_direction, SINGLE);
            
        }
    }
    
    
}