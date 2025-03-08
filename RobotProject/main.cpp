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
#include "../Dependencies/Auto/Paths/WaitForStartButton.h"
#include "../Dependencies/Auto/Paths/PathTest.h"
#include "../Dependencies/Auto/Paths/PathTest2.h"
#include "../Dependencies/Auto/Paths/SelectButton.h"
#include "../Dependencies/Auto/Paths/Rotate90Left.h"
#include "../Dependencies/Auto/Paths/AlignInCorner.h"
#include "../Dependencies/Auto/Paths/BackUpFromButton.h"
#include "../Dependencies/Auto/Paths/PressStartButton.h"
#include "../Dependencies/Auto/Paths/FinalButton.h"
#include "../Dependencies/Auto/Paths/Rotate45Right.h"
#include "../Dependencies/Auto/Paths/Rotate90Left2.h"
//menu selector________________________________________________________
enum class Menu : uint8_t {
    Idle,
    Auto,
    Joystick,
    PIDTuning,
    PoseEstimate
};

//Idle by default
Menu menuMode = Menu::Idle;


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

    //cds cell
    AnalogInputPin CDS(FEHIO::P2_0);

    //AutoInit_________________________________________________________________________________________________________
    //Sequencial command group
    SequencialCommand autonomous;

    //Auto sequences (add paths below):

    autonomous.addCommand(std::make_unique<WaitForStartButton>(CDS));
    autonomous.addCommand(std::make_unique<PressStartButton>(drivetrain));
    autonomous.addCommand(std::make_unique<Rotate45Right>(drivetrain));
    autonomous.addCommand(std::make_unique<PathTest>(drivetrain));
    autonomous.addCommand(std::make_unique<Rotate90Left>(drivetrain));
    autonomous.addCommand(std::make_unique<AlignInCorner>(drivetrain));
    autonomous.addCommand(std::make_unique<PathTest2>(drivetrain));
    autonomous.addCommand(std::make_unique<SelectButton>(drivetrain,CDS));
    autonomous.addCommand(std::make_unique<BackUpFromButton>(drivetrain));
    autonomous.addCommand(std::make_unique<Rotate90Left2>(drivetrain));
    autonomous.addCommand(std::make_unique<FinalButton>(drivetrain));
    
    
    
    


    float x_position, y_position;
    float x_trash, y_trash;
    bool velModeOn = false;

    float P = 0.6;
    float I = 0;
    float D = 0;



    //Code start________________________________________________________________________________________________________________

    //Prev loop clock
    auto start = std::chrono::steady_clock::now();

    
    Button autoModeButton(0,"Auto",BLUE,DARKBLUE);
    autoModeButton.setHeight(30);
    Button joystickModeButton(35,"Joystick",BLUE,DARKBLUE);
    joystickModeButton.setHeight(30);
    Button PIDTuningModeButton(70,"PID Tuning",BLUE,DARKBLUE);
    PIDTuningModeButton.setHeight(30);
    Button PoseEstimateButton(105,"Pose Estimate",BLUE,DARKBLUE);
    PoseEstimateButton.setHeight(30);
     
    //for PID tuning
    int motorSelected = 1;

    



//Code main loop__________________________________________________________________________________________________________________________________________________________________________________________
    while(true){
        LCD.Clear();
        /* Clear the touch buffer so touches made before
        this call are discarded */
        LCD.ClearBuffer();

        bool init = false;
        
            /* Wait until the user touches the screen */
            while(menuMode == Menu::Idle && !init) {
                joystickModeButton.updateButtonState();
                autoModeButton.updateButtonState();
                PIDTuningModeButton.updateButtonState();

                if(joystickModeButton.onButtonClicked()){
                    menuMode = Menu::Joystick;
                    init = true;
                }
                if(autoModeButton.onButtonClicked()){
                    menuMode = Menu::Auto;
                    init = true;
                }
                if(PIDTuningModeButton.onButtonClicked()){
                    menuMode = Menu::PIDTuning;
                    init = true;
                }
                if(PoseEstimateButton.onButtonClicked()){
                    menuMode = Menu::PoseEstimate;
                    init = true;
                }
            }

        drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

        LCD.Clear();
        LCD.ClearBuffer();

        bool exit = false;
        while(!exit){

            


            
            


            // LCD.WriteAt("Front Serial Speed: ", 0, 165);
            // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 180);

            // LCD.WriteAt("Front Serial Speed: ", 0, 195);
            // LCD.WriteAt(drivetrain.getSerialSpeed(), 0, 210);


            if(menuMode == Menu::Auto){
                //measure loopspeed
                float timeNow = TimeNowMSec();



                
                drivetrain.toggleVelocityControl(true);

                LCD.WriteAt("Elapsed Time: ",0, 180+30);
                LCD.WriteAt(timeNow,0, 195+30);
                LCD.WriteAt("Current Command: ",0,150+30);
                LCD.WriteAt(autonomous.getCurrentCommandName(),0,165+30);
                
                //run auto
                autonomous.runSequencialCommand();

                if(autonomous.ended()){
                    exit = true;
                    drivetrain.stop();
                }
            } else if (menuMode == Menu::PIDTuning){
                //PID Tuning
                //Telemetry____________________________________________________________________________________________________
                // float telemetryLineOffsetVel = 0;
                // LCD.WriteAt("Front Vel:",0,0+telemetryLineOffsetVel);
                // LCD.WriteAt(drivetrain.getFrontVelocity(),0,15+telemetryLineOffsetVel);
                
                // LCD.WriteAt("Back Left Vel",0,30+telemetryLineOffsetVel);
                // LCD.WriteAt(drivetrain.getBackLeftVelocity(),0,45+telemetryLineOffsetVel);

                // LCD.WriteAt("Back Right Vel",0,60+telemetryLineOffsetVel);
                // LCD.WriteAt(drivetrain.getBackRightVelocity(),0,75+telemetryLineOffsetVel);

                //joystick code
                float PIDTelemAdjust = 220;
                Button P_UP(15,0, "inc P",RED,DARKGREEN);
                P_UP.setHeight(30);
                P_UP.updateButtonState();
                Button I_UP(15,35, "inc I",GREEN,DARKGREEN);
                I_UP.setHeight(30);
                I_UP.updateButtonState();
                Button D_UP(15,70, "inc D",CYAN,DARKGREEN);
                D_UP.setHeight(30);
                D_UP.updateButtonState();

                Button P_DOWN(15+PIDTelemAdjust,0, "dec P",RED,DARKRED);
                P_DOWN.setHeight(30);
                P_DOWN.updateButtonState();
                Button I_DOWN(15+PIDTelemAdjust,35, "dec I",GREEN,DARKRED);
                I_DOWN.setHeight(30);
                I_DOWN.updateButtonState();
                Button D_DOWN(15+PIDTelemAdjust,70, "dec D",CYAN,DARKRED);
                D_DOWN.setHeight(30);
                D_DOWN.updateButtonState();

                // Adjust P value
                if (P_UP.onButtonClicked()) {
                    P += 0.1;
                }
                if (P_DOWN.onButtonClicked()) {
                    P -= 0.1;
                }

                // Adjust I value
                if (I_UP.onButtonClicked()) {
                    I += 0.001;
                }
                if (I_DOWN.onButtonClicked()) {
                    I -= 0.001;
                }

                // Adjust D value
                if (D_UP.onButtonClicked()) {
                    D += 0.001;
                }
                if (D_DOWN.onButtonClicked()) {
                    D -= 0.001;
                }

                
                // std::string motorSelection = "M: " + motorSelected;
                // Button MotorSelector(110, motorSelection, YELLOW);
                // MotorSelector.setHeight(30);
                // MotorSelector.updateButtonState();
                // if(MotorSelector.onButtonClicked()){
                //     motorSelected++;
                //     if(motorSelected > 3){
                //         motorSelected = 1;
                //     }
                //     MotorSelector.updateButtonState();
                // }

                

                


                // Update motor PID settings
                drivetrain.setMotorPID(1,P,I,D);
                drivetrain.setMotorPID(2,P,I,D);
                drivetrain.setMotorPID(3,P,I,D);

                LCD.WriteAt("P: ", 100, 180);
                LCD.WriteAt(P, 120, 180);

                LCD.WriteAt("I: ", 100, 200);
                LCD.WriteAt(I, 120, 200);

                LCD.WriteAt("D: ", 100, 220);
                LCD.WriteAt(D, 120, 220);


                LCD.WriteAt("TV: ", 0, 100);
                LCD.WriteAt(drivetrain.getFrontTargetVel(), 40, 100);

                LCD.WriteAt("AV: ", 0, 120);
                LCD.WriteAt(drivetrain.getFrontVelocity(), 40, 120);

                LCD.WriteAt("PO: ", 0, 140);
                LCD.WriteAt(drivetrain.getFrontPIDOut(), 40, 140);


                float telemetryOffsetTwo = 150;
                float telemetryYPos = 120+60;
                LCD.WriteAt("MovVecX ",telemetryYPos,90+telemetryOffsetTwo);
                LCD.WriteAt(movementVector[0],telemetryYPos,105+telemetryOffsetTwo);
                LCD.WriteAt("MovVecY",telemetryYPos,120+telemetryOffsetTwo);
                LCD.WriteAt((movementVector[1]),telemetryYPos,135+telemetryOffsetTwo);


                

                float joystickCenterX = (320/2.0);
                float joystickCenterY = (240/2.0);

                drivetrain.toggleVelocityControl(true);
                //show joystick area
                unsigned int joystickSize = 50;
                LCD.SetFontColor(BLUE);
                LCD.DrawEllipse(joystickCenterX,joystickCenterY,joystickSize,joystickSize);
                if(LCD.Touch(&x_position,&y_position)){
                    
                    if(checkInEllipse(x_position,y_position,joystickCenterX,joystickCenterY,joystickSize,joystickSize)){
                        //y is reversed bc of LCD y direction pos is down
                        //but I dont reverse it bc the proteus screen is facing out towards the front
                        //so the perspective of the joystick, up moves the bot back
                        //and so x is reversed

                        //calculate movement vector
                        movementVector[0] = -((x_position - joystickCenterX) / joystickSize); 
                        
                        movementVector[1] = ((y_position - joystickCenterY) / joystickSize);
                        //update rotation

                        drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

                        drivetrain.update();
                    }

                }else{
                    drivetrain.stop();
                }

            } else if (menuMode == Menu::PoseEstimate){
                float telemetryLineOffsetVel = 0;
                float telemetryLineOffsetEncoder = 95;
                LCD.WriteAt("Pose x:",0,0+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getPose()[0],0,15+telemetryLineOffsetVel);
                
                LCD.WriteAt("Pose y",0,30+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getPose()[1],0,45+telemetryLineOffsetVel);

                LCD.WriteAt("Pose theta",0,60+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getPose()[2],0,75+telemetryLineOffsetVel);








                float joystickCenterX = (320/2.0);
                float joystickCenterY = (240/2.0);

                drivetrain.toggleVelocityControl(true);
                //show joystick area
                unsigned int joystickSize = 50;
                LCD.SetFontColor(BLUE);
                LCD.DrawEllipse(joystickCenterX,joystickCenterY,joystickSize,joystickSize);
                if(LCD.Touch(&x_position,&y_position)){
                    
                    if(checkInEllipse(x_position,y_position,joystickCenterX,joystickCenterY,joystickSize,joystickSize)){
                        //y is reversed bc of LCD y direction pos is down
                        //but I dont reverse it bc the proteus screen is facing out towards the front
                        //so the perspective of the joystick, up moves the bot back
                        //and so x is reversed

                        //calculate movement vector
                        movementVector[0] = -((x_position - joystickCenterX) / joystickSize); 
                        
                        movementVector[1] = ((y_position - joystickCenterY) / joystickSize);
                        //update rotation

                        drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

                        drivetrain.update();
                    }

                }else{
                    drivetrain.stop();
                }

            }else{

                
                //menuMode == Menu::Joystick
                //Telemetry____________________________________________________________________________________________________
                float telemetryLineOffsetVel = 0;
                float telemetryLineOffsetEncoder = 95;
                LCD.WriteAt("Front Vel:",0,0+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getFrontVelocity(),0,15+telemetryLineOffsetVel);
                
                LCD.WriteAt("Back Left Vel",0,30+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getBackLeftVelocity(),0,45+telemetryLineOffsetVel);

                LCD.WriteAt("Back Right Vel",0,60+telemetryLineOffsetVel);
                LCD.WriteAt(drivetrain.getBackRightVelocity(),0,75+telemetryLineOffsetVel);

                LCD.WriteAt("Front Pos:",0,0+telemetryLineOffsetEncoder);
                LCD.WriteAt(drivetrain.getFrontPosition(),0,15+telemetryLineOffsetEncoder);
                
                LCD.WriteAt("Back Left Pos",0,30+telemetryLineOffsetEncoder);
                LCD.WriteAt(drivetrain.getBackLeftPosition(),0,45+telemetryLineOffsetEncoder);

                LCD.WriteAt("Back Right Pos",0,60+telemetryLineOffsetEncoder);
                LCD.WriteAt(drivetrain.getBackRightPosition(),0,75+telemetryLineOffsetEncoder);

                LCD.WriteAt("CDS Cell",0,90+telemetryLineOffsetEncoder);
                LCD.WriteAt(CDS.Value(),0,105+telemetryLineOffsetEncoder);


                float telemetryOffsetTwo = 60;
                float telemetryYPos = 120+60;

                LCD.WriteAt("MovVecX ",telemetryYPos,90+telemetryOffsetTwo);
                LCD.WriteAt(movementVector[0],telemetryYPos,105+telemetryOffsetTwo);
                LCD.WriteAt("MovVecY",telemetryYPos,120+telemetryOffsetTwo);
                LCD.WriteAt((movementVector[1]),telemetryYPos,135+telemetryOffsetTwo);


                //Vel/Pow mode buttons
                Button velMode(250,60, "Vel",GREEN,DARKBLUE);
                velMode.setHeight(30);
                Button powMode(250,95, "Pow",CYAN,DARKBLUE);
                powMode.setHeight(30);

                
                velMode.updateButtonState();
                powMode.updateButtonState();


                if(powMode.getButtonTriggered()){
                    velModeOn = false;
                    // drivetrain.setMotorsToRunToPositionMode();
                }
                if(velMode.getButtonTriggered()){
                    velModeOn = true;
                    LCD.Clear();
                    // drivetrain.setMotorsToRunAtVelocityMode();
                }
                

                drivetrain.toggleVelocityControl(velModeOn);

                //joystick code

                

                float joystickCenterX = (320/2.0);
                float joystickCenterY = (240/2.0);

                //show joystick area
                unsigned int joystickSize = 50;
                LCD.SetFontColor(BLUE);
                LCD.DrawEllipse(joystickCenterX,joystickCenterY,joystickSize,joystickSize);
                if(LCD.Touch(&x_position,&y_position)){
                    
                    if(checkInEllipse(x_position,y_position,joystickCenterX,joystickCenterY,joystickSize,joystickSize)){
                        //y is reversed bc of LCD y direction pos is down
                        //but I dont reverse it bc the proteus screen is facing out towards the front
                        //so the perspective of the joystick, up moves the bot back
                        //and so x is reversed

                        //calculate movement vector
                        movementVector[0] = -((x_position - joystickCenterX) / joystickSize); 
                        
                        movementVector[1] = ((y_position - joystickCenterY) / joystickSize);
                        //update rotation

                        drivetrain.setMovementVector(movementVector[0],movementVector[1],movementVector[2]);

                        drivetrain.update();
                    }
                }else{
                    drivetrain.stop();
                }
            }
            

            


        }
        menuMode = Menu::Idle;
    }

	return 0;
}
