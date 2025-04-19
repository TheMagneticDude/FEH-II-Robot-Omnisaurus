#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AppleBasket.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AppleBasket::AppleBasket(HolonomicTriangleDrive &dt, FEHServo &a, OptoSensorArray &opt) : drivetrain(dt), arm(a), opto(opt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void AppleBasket::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(-3,0,-150);
    drivetrain.resetMotorCounts();
    drivetrain.toggleVelocityControl(true);
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
        //turn back so heading is 0
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,1750)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0  deg
            drivetrain.setPose(-3,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        //move back to align against wall
        drivetrain.setTargetPose(-3,-10,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime, 1000)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(drivetrain.getPose()[0],0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //move towards apple basket
        drivetrain.setTargetPose(-5,8.5,0);
        drivetrain.runToPoseLim(1);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime, 4000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        arm.SetDegree(136);
        //extend arm and move towards apple basket
        drivetrain.setTargetPose(-9,8.6,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime, 4000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++;
            drivetrain.stop();
        }
        break;

        case 4:
        //raise arm
        arm.SetDegree(125);
        //can lower time limit
        if(timeUp(startTime, 300)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        //raise arm (split up so its slower)
        arm.SetDegree(110);
        //can lower time limit
        if(timeUp(startTime, 300)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 6:
        //raise arm (split up so its slower)
        arm.SetDegree(100);
        //can lower time limit
        if(timeUp(startTime, 300)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 7:
        //raise arm (split up so its slower)
        arm.SetDegree(0);
        //can lower time limit
        if(timeUp(startTime, 100)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 8:

        //back up from apple  basket
        drivetrain.setTargetPose(-7,9,0);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime, 4000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++;
            drivetrain.stop();
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