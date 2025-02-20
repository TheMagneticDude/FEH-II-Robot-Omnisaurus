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
#include <cmath>
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"

//ROBOT MAIN
//Motor M1 should be Motor 0
//M2(BackLeft) should be Motor 1
//M3(BackRight) should be Motor 3 (the connectors are too big to fit all three next to each other...)
int main(void)
{

    float movementVector[3] = {0.8,0,0};
    LCD.Clear(BLACK);
    LCD.WriteAt("Movement Vector: X: ",0,0);
    LCD.WriteAt(movementVector[0],0,15);

    LCD.WriteAt("Movement Vector: Y: ",0,30);
    LCD.WriteAt(movementVector[1],0,45);

    LCD.WriteAt("Movement Vector: Rot: ",0,60);
    LCD.WriteAt(movementVector[2],0,75);

    float motorMaxVolt = 6.0;//should be 9, im testing with lower voltage cap for now

    // FEHMotor M1(FEHMotor::Motor0,motorMaxVolt);
    // FEHMotor M2(FEHMotor::Motor1,motorMaxVolt);
    // FEHMotor M3(FEHMotor::Motor3,motorMaxVolt);

    FEHMotor::FEHMotorPort M1(FEHMotor::Motor0);
    FEHMotor::FEHMotorPort M2(FEHMotor::Motor1);
    FEHMotor::FEHMotorPort M3(FEHMotor::Motor3);
    
    HolonomicTriangleDrive drivetrain(M1,M2,M3,motorMaxVolt);


    float x_position, y_position;
    float x_trash, y_trash;

        /* Clear the touch buffer so touches made before
           this call are discarded */
           LCD.ClearBuffer();
           /* Wait until the user touches the screen */
           while(!LCD.Touch(&x_position,&y_position)) {};
           /* Wait until the touch releases */
           while(LCD.Touch(&x_trash,&y_trash)) {};

    drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

    LCD.Clear();
    while(true){
        LCD.WriteAt("Front Speed:",0,0);
        LCD.WriteAt(drivetrain.getFrontSpeed(),0,90);
        
        LCD.WriteAt("Back Left Speed",0,105);
        LCD.WriteAt(drivetrain.getBackLeftSpeed(),0,120);

        LCD.WriteAt("Back Right Speed",0,135);
        LCD.WriteAt(drivetrain.getBackRightSpeed(),0,150);

        drivetrain.update();
    }

	return 0;
}
