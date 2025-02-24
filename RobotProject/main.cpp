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
#include <chrono>
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"
#include "../Dependencies/Auto/SequencialCommand.h"
#include "../Dependencies/Button.h"
#include "../Dependencies/Slider.h"

#include <pthread.h>
#include <iostream>

//Import Auto Paths_________________________________________________
#include "../Dependencies/Auto/Paths/PathTest.h"
#include "../Dependencies/Auto/Paths/PathTest2.h"


//helper methods_____________________________________________________
    bool checkInEllipse(int x, int y, int centerx, int centery, int width, int height) {
    // taken from the equation of an ellipse
    // if it is greater than 1 then its not in the ellipse
    // recenters ellipse;
    double dx = x - centerx;
    double dy = y - centery;
    // Check if inside ellipse
    double p = (dx * dx / (width * width)) + (dy * dy / (height * height));
    return p <= 1.0;
}

    //ROBOT MAIN
    //Motor M1 should be Motor 0
    //M2(BackLeft) should be Motor 1
    //M3(BackRight) should be Motor 3 (the connectors are too big to fit all three next to each other...)
int main(void)
{
    //Variables_________________________________________________________________________________________________________
    float movementVector[3] = {0,0,0};
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
    autonomous.addCommand(std::make_unique<PathTest2>(drivetrain));


    float x_position, y_position;
    float x_trash, y_trash;





    //Code start________________________________________________________________________________________________________________

    //Prev loop clock
    auto start = std::chrono::steady_clock::now();

    //default is joystick control mode
    bool autoMode = false;
    Button joystickModeButton(0,"Joystick",BLUE,DARKBLUE);
    joystickModeButton.setHeight(30);
    Button autoModeButton(35,"Auto",BLUE,DARKBLUE);
    autoModeButton.setHeight(30);
    Slider rotationControl(0, 100,0,BLUE,DARKBLUE,-1,1);

    LCD.Clear();
    /* Clear the touch buffer so touches made before
    this call are discarded */
    LCD.ClearBuffer();

    bool init = false;
        /* Wait until the user touches the screen */
        while(!init) {
            joystickModeButton.updateButtonState();
            autoModeButton.updateButtonState();

            if(joystickModeButton.onButtonClicked()){
                autoMode = false;
                init = true;
            }
            if(autoModeButton.onButtonClicked()){
                autoMode = true;
                init = true;
            }
        }

    drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

    LCD.Clear();
    LCD.ClearBuffer();

    bool exit = false;
    while(!exit){

        //measure loopspeed
        float timeNow = TimeNowMSec();


        float telemetryLineOffset = 20;
        //Telemetry____________________________________________________________________________________________________
        // LCD.WriteAt("Front Pos:",0,0+telemetryLineOffset);
        // LCD.WriteAt(drivetrain.getFrontPosition(),0,15+telemetryLineOffset);
        
        // LCD.WriteAt("Back Left Pos",0,30+telemetryLineOffset);
        // LCD.WriteAt(drivetrain.getBackLeftPosition(),0,45+telemetryLineOffset);

        // LCD.WriteAt("Back Right Pos",0,60+telemetryLineOffset);
        // LCD.WriteAt(drivetrain.getBackRightPosition(),0,75+telemetryLineOffset);

        LCD.WriteAt("Elapsed Time: ",0, 180+30);
        LCD.WriteAt(timeNow,0, 195+30);


        
        


        // LCD.WriteAt("Front Serial Speed: ", 0, 165);
        // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 180);

        // LCD.WriteAt("Front Serial Speed: ", 0, 195);
        // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 210);


        if(autoMode){
            LCD.WriteAt("Current Command: ",0,150+30);
            LCD.WriteAt(autonomous.getCurrentCommandName(),0,165+30);
            
            //run auto
            autonomous.runSequencialCommand();

            if(autonomous.ended()){
                exit = true;
                drivetrain.stop();
            }
        }else{

            float telemetryOffsetTwo = 30;

            LCD.WriteAt("TouchX ",0,90+telemetryOffsetTwo);
            LCD.WriteAt(x_position,0,105+telemetryOffsetTwo);
            LCD.WriteAt("TouchY",0,120+telemetryOffsetTwo);
            LCD.WriteAt((-y_position),0,135+telemetryOffsetTwo);


            //joystick code

            //render rotation slider
            rotationControl.update();
            float sliderVal = rotationControl.getValue();

            float joystickCenterX = (320/2.0);
            float joystickCenterY = (240/2.0);

            //show joystick area
            unsigned int joystickSize = 50;
            LCD.SetFontColor(BLUE);
            LCD.DrawEllipse(joystickCenterX,joystickCenterY,joystickSize,joystickSize);
            if(LCD.Touch(&x_position,&y_position)){
                if(checkInEllipse(x_position,y_position,joystickCenterX,joystickCenterY,joystickSize,joystickSize)){

                    //calculate movement vector
                    movementVector[0] = ((x_position - joystickCenterX) / joystickSize); 
                    movementVector[1] = ((y_position - joystickCenterY) / joystickSize);
                    //update rotation
                    if(rotationControl.getHeld()){
                        //sets angular velocity
                        //-1 to 1
                        movementVector[2] = sliderVal;
                    }

                    drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

                    drivetrain.update();
                }
            }else{
                drivetrain.stop();
            }
        }
        

        


    }

	return 0;
}
