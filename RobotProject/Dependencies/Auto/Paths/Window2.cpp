#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Window2.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Window2::Window2(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void Window2::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(-12,0,-60);//assuming the robot is about aligned with the mechanism facing 60 deg bc when it pushes thats how it aligns
    drivetrain.resetMotorCounts();
    drivetrain.toggleVelocityControl(true);
}

//Runs the command every tick
void Window2::run(){
    
    //Command stuff
    std::string s = "Window2 SubPath: " + i;
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
        //move away from window
        drivetrain.setTargetPose(-11,1,-60);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        
        case 1:
        //turn back (to -90 deg)
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,300)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(0,1,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //move to other side of window to align straight
        drivetrain.setTargetPose(-14,1,-60);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,1,-90); 
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
        }
        break;

        case 3:
        //reset to 0,0 at window corner
        drivetrain.setTargetPose(-3,-8,-90);
        drivetrain.toggleVelocityControl(false);
        drivetrain.runTilStalled(0.4);
        if(drivetrain.isCurrStalled() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,1,-90); 
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
            drivetrain.toggleVelocityControl(true);
            drivetrain.resetStallDetection();
        }
        break;

        case 4:
        //move window open
        drivetrain.setTargetPose(3,-1,-90);
        drivetrain.toggleVelocityControl(false);
        drivetrain.runTilStalled(0.8);
        if(drivetrain.isCurrStalled() || timeUp(startTime,1200)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.toggleVelocityControl(true);
            drivetrain.resetStallDetection();
        }
        break;

        case 5:
        //turn back (to -90 deg)
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,300)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(0,1,-90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 6:
        //move to other side of window to align straight again
        drivetrain.setTargetPose(0,2,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
        }
        break;

        case 7:
        //realign at wall
        drivetrain.setTargetPose(-15,0,-90);
        drivetrain.toggleVelocityControl(false);
        drivetrain.runTilStalled(0.4);
        if(drivetrain.isCurrStalled() || timeUp(startTime,2000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.toggleVelocityControl(true);
            drivetrain.resetStallDetection();
        }
        break;

        case 9:
        //realign at other wall
        drivetrain.setTargetPose(0,-15,-90);
        drivetrain.toggleVelocityControl(false);
        drivetrain.runTilStalled(0.4);
        if(drivetrain.isCurrStalled() || timeUp(startTime,2000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.toggleVelocityControl(true);
            drivetrain.resetStallDetection();
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
bool Window2::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void Window2::stop(){end = true;}

//returns path name
std::string Window2::getName(){return commandName;}