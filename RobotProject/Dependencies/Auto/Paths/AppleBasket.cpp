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
    drivetrain.setPose(0,0,-45);
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
        // case 0:
        // //path 1
        // arm.SetDegree(180);
        // //reverse from composter
        // drivetrain.setTargetPose(-2,0.5,-150);
        // drivetrain.runToPose();
        // if(drivetrain.getReachedTargetPos()){
        //     drivetrain.setPose(-2,0.5,-150);
        //     startTime = TimeNowMSec();
        //     i++; 
        // }
        // break;


        case 0:
        //move towards apple basket
        drivetrain.setTargetPose(-7,7,-45);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setPose(-7, 7, -45);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        arm.SetDegree(180);
        //turn right 45 so back is straight with wall (so forwards is -x))
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,400)){
            drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(-7,7,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        

        case 2:
        //go towards apple basket
        arm.SetDegree(120);
        drivetrain.setTargetPose(-12,7,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        arm.SetDegree(0);
        //pick up basket
        //can add a delay
        if(timeUp(startTime,0)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        //move away from apple bucket
        drivetrain.setTargetPose(-7,7,0);
        drivetrain.runToPoseLim(0.6);
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

// case 5:
// //rotate 90 to face apple basket
// drivetrain.setMovementVector(0,0,-0.5);
// drivetrain.update();
// if(timeUp(startTime,800)){
//     drivetrain.setPose(0,7,0);
//     drivetrain.setMovementVector(0,0,0);
//     startTime = TimeNowMSec();
//     i++; 
// }
// break;

        // case 3:
        // //rotate 90 to align with wall
        // drivetrain.setMovementVector(0,0,0.5);
        // drivetrain.update();
        // if(timeUp(startTime,800)){
        //     drivetrain.setPose(-2,7,-90);
        //     drivetrain.setMovementVector(0,0,0);
        //     startTime = TimeNowMSec();
        //     i++; 
        // }
        // break;

        // case 4:
        
        // //move sideways towards apple basket until optosensors are triggered
        // drivetrain.setMovementVector(-0.3,0,0);
        // drivetrain.update();
        // if(opto.getLState() || opto.getRState() || opto.getMState()){
        //     drivetrain.setMovementVector(0,0,0);
        //     drivetrain.setPose(0,7,0);//apple basket is at x = 0 now
        //     startTime = TimeNowMSec();
        //     i++; 
        // }
        // break;

//exit condition, returns true once command sequence has ended
bool AppleBasket::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AppleBasket::stop(){end = true;}

//returns path name
std::string AppleBasket::getName(){return commandName;}