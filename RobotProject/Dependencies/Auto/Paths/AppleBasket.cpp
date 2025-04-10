#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AppleBasket.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AppleBasket::AppleBasket(HolonomicTriangleDrive &dt, FEHServo &a) : drivetrain(dt), arm(a){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void AppleBasket::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(-6,0.5,-150);
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void AppleBasket::run(){
    
    //Command stuff
    std::string s = "AppleBasket SubPath: " + i;
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = "Elapsed: " + std::to_string(elapsed) + " ms";

    LCD.WriteAt(elapsedS.c_str(),0,30);


    stringstream pose;
    pose << "Po: [" << std::fixed << std::setprecision(2) << drivetrain.getPose()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getPose()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getPose()[2] << "]";
    LCD.WriteAt(pose.str(),0,60);

    stringstream targetPose;
    targetPose << "TG: [" << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getTargetPos()[2] << "]";
    LCD.WriteAt(targetPose.str(),0,75);

    stringstream movementVector;
    movementVector << "MV: [" << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[0] << ", " << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[1] << ", " << std::fixed << std::setprecision(2) << drivetrain.getMovementVector()[2] << "]";
    LCD.WriteAt(movementVector.str(),0,90);


    
    switch(i){
        case 0:
        //path 1
        arm.SetDegree(180);
        //reverse from composter
        drivetrain.setTargetPose(-2,0.5,-150);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setPose(-6,0.5,-150);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        //turn back to zero
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,1240)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(-2,0.5,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //move back to aling with wall
        drivetrain.setTargetPose(-2,-5,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            // drivetrain.setMovementVector(0,0,0);
            //should now be aligned at y = 0
            drivetrain.setPose(drivetrain.getPose()[0], 0, 0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        //move forwards until in line with apple basket
        drivetrain.setTargetPose(-2,6,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos()){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        arm.SetDegree(120);
        //move sideways towards apple basket
        drivetrain.setTargetPose(-6,6,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos()){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        arm.SetDegree(120);
        //move back to original pos: prepare for window path
        drivetrain.setTargetPose(-2,6,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos()){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool AppleBasket::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AppleBasket::stop(){end = true;}

//returns path name
std::string AppleBasket::getName(){return commandName;}