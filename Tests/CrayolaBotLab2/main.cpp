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
    return (x >= min) && (x <= max);
}

bool threshold(float x, float e){
    return (x > e);
}






int main(void)
{

    float onLine[3] = {0,0,0};
    float onBack[3] = {0,0,0};
    
    



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

    DigitalInputPin Limit(FEHIO::P3_0);


    float speed =  20.0;
    float slowConst = 0.5;

    //nonreflective is 3.3V
    //reflective is 0
    //threshold
    //LINE is 2, not on line is 1
    float L_OptiEpsilon = 3.3;
    float M_OptiEpsilon = 3.3;
    float R_OptiEpsilon = 3.3;

    float L_OptiEpsilon_Min = 1.9;
    float M_OptiEpsilon_Min = 1.9;
    float R_OptiEpsilon_Min = 1.9;

    LCD.Clear(BLACK);

        int state = MIDDLE; // Set the initial state
    while (true) { // I will follow this line forever!
        
        
        LCD.WriteAt("Opti_L: ",0,0);
        LCD.WriteAt((L_Opti.Value()),0,15);
        LCD.WriteAt("Opti_M: ",0,30);
        LCD.WriteAt((M_Opti.Value()),0,45);
        LCD.WriteAt("Opti_R: ",0,60);
        LCD.WriteAt((R_Opti.Value()),0,75);



        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                onLine[0] = L_Opti.Value();
                onLine[1] = M_Opti.Value();
                onLine[2] = R_Opti.Value();

                LCD.WriteAt("State: Middle",0,90);

                right_motor.SetPercent(-speed);
                left_motor.SetPercent(speed);
                /* Drive */
                if (isWithin(R_Opti.Value(),R_OptiEpsilon_Min,R_OptiEpsilon)/* Right sensor is on line */ ) {
                state = RIGHT; // update a new state
                } 
                if (isWithin(L_Opti.Value(),L_OptiEpsilon_Min,L_OptiEpsilon)/* Left sensor is on line */ ) {
                    state = LEFT; // update a new state
                    } 
                break; 
                // If the right sensor is on the line... 
            case RIGHT:
                onBack[0] = L_Opti.Value();
                onBack[1] = M_Opti.Value();
                onBack[2] = R_Opti.Value();
            LCD.WriteAt("State: Right",0,90);
                // Set motor powers for right turn
                right_motor.SetPercent(-speed);
                left_motor.SetPercent(speed*slowConst);
                /* Drive */

            if(!isWithin(R_Opti.Value(),R_OptiEpsilon_Min,R_OptiEpsilon)/* I no longer need to turn right… */ ) { 
                state = MIDDLE;
                } 
                break; 
                // If the left sensor is on the line... 
            case LEFT:
                onBack[0] = L_Opti.Value();
                onBack[1] = M_Opti.Value();
                onBack[2] = R_Opti.Value();
            LCD.WriteAt("State: Left",0,90);
                /* Mirror operation of LEFT state */

                // Set motor powers for left turn
                right_motor.SetPercent(-speed*slowConst);
                left_motor.SetPercent(speed);
                /* Drive */

            if(!isWithin(L_Opti.Value(),L_OptiEpsilon_Min,L_OptiEpsilon)/* I no longer need to turn left */ ) { 
                state = MIDDLE;
                }
                break; 
                default: // Error. Something is very wrong.
                LCD.WriteLine("AAAAAAAAAAAAAAAAAAAAAAAAAAAaaAAaAAaAaAaAaAaAAaAA ERROR");
                break; 
        } 

            if(!Limit.Value()){
            //Open output log file
            FEHFile *ofptr = SD.FOpen("OutputLog.txt", "w");
            //Print data using formatted string
            SD.FPrintf(ofptr, "INT: %d, FLOAT: %f, CHAR: %c", onLine[0], onLine[1], onLine[3]);
            SD.FPrintf(ofptr, "INT: %d, FLOAT: %f, CHAR: %c", onBack[0], onBack[1], onBack[3]);
            //Close output log file
            SD.FClose(ofptr);
            }
            
    }


    return 0;
}
