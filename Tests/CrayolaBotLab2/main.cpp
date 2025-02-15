#include <FEHLCD.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>



bool isWithin(float x, float min, float max){
    return (x >= min) || (x <= max);
}

bool threshold(float x, float e){
    return (x > e);
}






int main(void)
{
    FEHMotor right_motor(FEHMotor::Motor1,9.0);
    FEHMotor left_motor(FEHMotor::Motor2, 9.0);
    
    enum LineStates { 
        MIDDLE, 
        RIGHT, 
        LEFT
       };

    AnalogInputPin L_Opti(FEHIO::P0_0);
    AnalogInputPin M_Opti(FEHIO::P0_1);
    AnalogInputPin R_Opti(FEHIO::P0_2);


    float speed =  40.0;

    //nonreflective is 3.3V
    //reflective is 0
    //threshold
    float L_OptiEpsilon = 1;
    float M_OptiEpsilon = 1;
    float R_OptiEpsilon = 1;


        int state = MIDDLE; // Set the initial state
    while (true) { // I will follow this line forever!
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                right_motor.SetPercent(speed);
                left_motor.SetPercent(speed);
                /* Drive */
                if (isWithin(R_Opti.Value(),0,R_OptiEpsilon)/* Right sensor is on line */ ) {
                state = RIGHT; // update a new state
                } 
                if (isWithin(L_Opti.Value(),0,L_OptiEpsilon)/* Left sensor is on line */ ) {
                    state = LEFT; // update a new state
                    } 
                break; 
                // If the right sensor is on the line... 
            case RIGHT:
                // Set motor powers for right turn
                right_motor.SetPercent(speed*0.75);
                left_motor.SetPercent(speed);
                /* Drive */

            if(!isWithin(R_Opti.Value(),0,R_OptiEpsilon)/* I no longer need to turn right… */ ) { 
                state = MIDDLE;
                } 
                break; 
                // If the left sensor is on the line... 
            case LEFT:
                /* Mirror operation of LEFT state */

                // Set motor powers for left turn
                right_motor.SetPercent(speed);
                left_motor.SetPercent(speed*0.75);
                /* Drive */

            if(!isWithin(L_Opti.Value(),0,L_OptiEpsilon)/* I no longer need to turn left */ ) { 
                state = MIDDLE;
                }
                break; 
                default: // Error. Something is very wrong.
                LCD.WriteLine("AAAAAAAAAAAAAAAAAAAAAAAAAAAaaAAaAAaAaAaAaAaAAaAA ERROR");
                break; 
        } 
            // Sleep a bit
    }

    return 0;
}
