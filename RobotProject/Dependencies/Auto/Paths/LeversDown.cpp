#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "LeversDown.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
LeversDown::LeversDown(HolonomicTriangleDrive &dt, int &l, OptoSensorArray opsArr, FEHServo &a) : drivetrain(dt), OptoArr(opsArr), lever(l), arm(a){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void LeversDown::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(0,0,0);//starts in button corner
    drivetrain.resetMotorCounts();
    drivetrain.toggleVelocityControl(true);

    //init end flag
    end = false;
    didInit = false;
    CDSisRed = false;//defaults to blue
}

//Runs the command every tick
void LeversDown::run(){
    
    //Command stuff
    std::string s = "LeversDown SubPath: " + i;
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = "Elapsed: " + std::to_string(elapsed) + " ms";

    LCD.WriteAt(elapsedS.c_str(),0,30);

    LCD.WriteAt(lever,0,60);


    stringstream pose;
    pose << "Po: [" << std::fixed << std::setprecision(2) << drivetrain.getPose()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getPose()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getPose()[2] << "]";
    LCD.WriteAt(pose.str(),0,60);

    stringstream targetPose;
    targetPose << "TG: [" << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[2] << "]";
    LCD.WriteAt(targetPose.str(),0,75);

    stringstream movementVector;
    movementVector << "MV: [" << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[2] << "]";
    LCD.WriteAt(movementVector.str(),0,90);

    float CDS_None = 1;//0.8
    float CDS_Red = 2.9;//3.18
    float CDS_Blue = 2.6;//2.8
    
    switch(i){

        case 0:
            if(lever == 0){
                LCD.Clear();
                LCD.SetBackgroundColor(ORANGE);
            }
            if(lever == 1){
                LCD.Clear();
                LCD.SetBackgroundColor(GREEN);
            }
            if(lever == 2){
                LCD.Clear();
                LCD.SetBackgroundColor(CYAN);
            }
            if(timeUp(startTime,0)){
                drivetrain.setMovementVector(0,0,0);
                startTime = TimeNowMSec();
                i++; 
            }
        break;


        case 1:
        //starts in corner of the button place
        //move slightly away to rotate
        drivetrain.setTargetPose(2,2,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++;
        }
        break;


        case 2:
        //turn right 45 to align with levers
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,500)){
            drivetrain.setPose(2,2,45);
            drivetrain.setMovementVector(2,2,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;


        case 3:
        //move to levers
        drivetrain.setTargetPose(1,8,45);
        drivetrain.runToPoseLim(0.5);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2500)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        //lever down
        arm.SetDegree(180);

        if(timeUp(startTime,300)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        //lever down
        arm.SetDegree(180);
        drivetrain.setTargetPose(1,5,45);
        drivetrain.runToPoseLim(0.3);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.stop();
        }
        break;

        case 6:
        //wait for 5sec for lever
        arm.SetDegree(180);
        if(timeUp(startTime,5000)){
            startTime = TimeNowMSec();
            i++; 
            drivetrain.stop();
        }
        break;

        case 7:
        //move to levers again
        drivetrain.setTargetPose(1,8,45);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 8:
        //lever up
        arm.SetDegree(130);
        drivetrain.setTargetPose(1,5,45);
        drivetrain.runToPoseLim(0.3);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        
        case 9:
        //turn right 45 back to 0
        arm.SetDegree(0);
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,500)){
            drivetrain.setPose(drivetrain.getPose()[0],drivetrain.getPose()[1],0);
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            drivetrain.setPose(0,0,0);
            i++; 
        }
        break;

        case 10:
        //to towards window to reset
        drivetrain.setTargetPose(8,-5,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 11:
        //to towards window to reset
        drivetrain.setTargetPose(7,-15,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(7,0,0);
        }
        break;


        case 12:
        //align xy/wndow
        drivetrain.toggleVelocityControl(false);
        drivetrain.setTargetPose(-20,-10,0);
        drivetrain.runTilStalled(0.5);
        if(drivetrain.isCurrStalled() || timeUp(startTime,2500)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,0,-90);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 13:
        //move away from window to turn
        drivetrain.setTargetPose(0,4,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 14:
        //turn towards wall -90
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1050)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  -90 deg
            drivetrain.setPose(0,0,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 15:
        //reset against wall
        drivetrain.setTargetPose(35,0,-90);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2500)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(0,0,-90);
        }
        break;

    
        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool LeversDown::ended(){
    // LCD.Clear();
    // LCD.SetBackgroundColor(BLACK);
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void LeversDown::stop(){end = true;}

//returns path name
std::string LeversDown::getName(){return commandName;}