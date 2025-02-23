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
#include "../Dependencies/Auto/SequencialCommand.h"
#include "../Dependencies/Auto/Paths/PathTest.h"
#include "../Dependencies/Button.h"

//helper methods_____________________________________________________



    //ROBOT MAIN
    //Motor M1 should be Motor 0
    //M2(BackLeft) should be Motor 1
    //M3(BackRight) should be Motor 3 (the connectors are too big to fit all three next to each other...)
int main(void)
{
    //Variables_________________________________________________________________________________________________________
    float movementVector[3] = {0,0,0};
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

    FEHIO::FEHIOPin E1 = FEHIO::P0_0;
    FEHIO::FEHIOPin E2 = FEHIO::P0_2;
    FEHIO::FEHIOPin E3 = FEHIO::P0_4;
    
    HolonomicTriangleDrive drivetrain(M1,E1,M2,E2,M3,E3,motorMaxVolt);

    //AutoInit_________________________________________________________________________________________________________
    //Sequencial command group
    SequencialCommand autonomous;

    //Auto sequences (add paths below):
    autonomous.addCommand(std::make_unique<PathTest>(drivetrain));


    float x_position, y_position;
    float x_trash, y_trash;


    //Code start________________________________________________________________________________________________________________

    //default is joystick control mode
    bool autoMode = false;
    Button joystickModeButton(0,"Joystick",LCD.Blue);
    Button autoModeButton(15,"Auto",LCD.Blue);

        /* Clear the touch buffer so touches made before
           this call are discarded */
           LCD.ClearBuffer();
           /* Wait until the user touches the screen */
           while(!LCD.Touch(&x_position,&y_position)) {
            joystickModeButton.updateButtonState();
            autoModeButton.updateButtonState();

            if(joystickModeButton.onButtonClicked()){
                autoMode = false;
            }
            if(autoModeButton.onButtonClicked()){
                autoMode = true;
            }
           };
           /* Wait until the touch releases */
           while(LCD.Touch(&x_trash,&y_trash)) {
           };

    drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

    LCD.Clear();
    while(true){
        float telemetryLineOffset = 20;
        //Telemetry____________________________________________________________________________________________________
        LCD.WriteAt("Front Pos:",0,0+telemetryLineOffset);
        LCD.WriteAt(drivetrain.getFrontPosition(),0,15+telemetryLineOffset);
        
        LCD.WriteAt("Back Left Pos",0,30+telemetryLineOffset);
        LCD.WriteAt(drivetrain.getBackLeftPosition(),0,45+telemetryLineOffset);

        LCD.WriteAt("Back Right Pos",0,60+telemetryLineOffset);
        LCD.WriteAt(drivetrain.getBackRightPosition(),0,75+telemetryLineOffset);


        


        LCD.WriteAt("TouchX ",0,90+telemetryLineOffset);
        LCD.WriteAt(x_position,0,105+telemetryLineOffset);
        LCD.WriteAt("TouchY",0,135+telemetryLineOffset);
        LCD.WriteAt((-y_position),0,150+telemetryLineOffset);


        // LCD.WriteAt("Front Serial Speed: ", 0, 165);
        // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 180);

        // LCD.WriteAt("Front Serial Speed: ", 0, 195);
        // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 210);


        if(autoMode){
            //run auto
            autonomous.runSequencialCommand();
        }else{
            //joystick code
            if(LCD.Touch(&x_position,&y_position)){
                        

                movementVector[0] = ((x_position - (320/2.0)) / 320); 
                movementVector[1] = ((y_position - (240/2.0)) / 240);

                drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

                drivetrain.update();
            }else{
                drivetrain.stop();
            }
        }
        

        


    }

	return 0;
}
