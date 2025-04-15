#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AppleBasketDropoff.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AppleBasketDropoff::AppleBasketDropoff(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void AppleBasketDropoff::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(-7,9,0);
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void AppleBasketDropoff::run(){
    
    //Command stuff
    std::string s = "AppleBasketDropoff SubPath: " + i;
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
        //turn towards ramp wall (to 90 deg)
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  -90 deg
            drivetrain.setPose(-7,9,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        //move towards ramp
        drivetrain.setTargetPose(-4,5,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //move to wall next to ramp
        drivetrain.setTargetPose(6,5,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,5,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        //move -x some to rotate
        drivetrain.setTargetPose(5,5,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,500)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        //turn towards ramp (to 0 deg)
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(5,5,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        //move up ramp
        drivetrain.setTargetPose(5,50,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,6000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 6:
        //turn towards window to align in corner (to -90 deg)
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  -90 deg
            drivetrain.setPose(5,25,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 7:
        //align x
        drivetrain.setTargetPose(25,25,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,25,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 8:
        //move forwards slightly and align y
        drivetrain.setTargetPose(1,35,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 9:
        //align x and y
        drivetrain.setTargetPose(10,35,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,3000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            drivetrain.setPose(0,0,-90);//0,0 set in corner of apple basket top drawer
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
bool AppleBasketDropoff::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AppleBasketDropoff::stop(){end = true;}

//returns path name
std::string AppleBasketDropoff::getName(){return commandName;}