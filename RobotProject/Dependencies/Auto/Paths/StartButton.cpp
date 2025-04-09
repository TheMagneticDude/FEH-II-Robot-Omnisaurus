#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "StartButton.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
StartButton::StartButton(HolonomicTriangleDrive &dt) : drivetrain(dt){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void StartButton::init(){
    //starting circle is 0,0 with -x being sideways towards composter and +y being forwards towards ramp
    //robot starts at a -45 degree angle  
    drivetrain.resetMotorCounts();
    drivetrain.setPose(0,0,0);
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void StartButton::run(){
    
    //Command stuff
    std::string s = "Composter SubPath: " + std::to_string(i);
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = "Elapsed: " + std::to_string(elapsed) + " ms";
    // LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    // LCD.WriteAt("ms",0,45);

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
        //move back to hit start
        drivetrain.setTargetPose(0,-3,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        

        case 1:
        //move forward again to return to original position
        drivetrain.setTargetPose(0,0,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setMovementVector(0,0,0);
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
bool StartButton::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void StartButton::stop(){end = true;}

//returns path name
std::string StartButton::getName(){return commandName;}