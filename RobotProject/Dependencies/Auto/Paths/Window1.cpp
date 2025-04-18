#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Window1.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Window1::Window1(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void Window1::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(0,6,-90);
    drivetrain.resetMotorCounts();
    drivetrain.toggleVelocityControl(true);
}

//Runs the command every tick
void Window1::run(){
    
    //Command stuff
    std::string s = "Window1 SubPath: " + i;
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
        //align against window
        drivetrain.setTargetPose(-2,-20,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        //move window open
        //TODO: NEED TO ADD INPUT FROM WHETHER WINDOW IS OPEN TO KEEP GOING
        drivetrain.setTargetPose(-11,-1,-90);
        drivetrain.toggleVelocityControl(false);
        drivetrain.runTilStalled(0.8);
        if(drivetrain.isCurrStalled() || timeUp(startTime,1200)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++;
            drivetrain.resetStallDetection();
            drivetrain.toggleVelocityControl(true);
        }
        break;

        

        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool Window1::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void Window1::stop(){end = true;}

//returns path name
std::string Window1::getName(){return commandName;}