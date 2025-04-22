#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "ReturnToStart.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
ReturnToStart::ReturnToStart(HolonomicTriangleDrive &dt) : drivetrain(dt){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void ReturnToStart::init(){
    drivetrain.setPose(0,6,-90);
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.resetMotorCounts();
    drivetrain.toggleVelocityControl(true);
}

//Runs the command every tick
void ReturnToStart::run(){
    //Command stuff
    std::string s =  commandName + " SubPath: " + std::to_string(i);
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
        //move forwards one bit
        drivetrain.setTargetPose(-1,0,-90);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.isCurrStalled() || timeUp(startTime,500)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        

        case 1:
        //turn towards ramp (to -180 deg)
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1100)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(-1,0,-180);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //run down ramp
        drivetrain.setTargetPose(-1,-50,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.isCurrStalled() || timeUp(startTime,10000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(0,0,0);
        }
        break;

        case 3:
        //run towards final button
        drivetrain.setTargetPose(-3,0,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.isCurrStalled() || timeUp(startTime,5000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(0,0,0);
        }
        break;

        case 4:
        //run towards final button again
        drivetrain.setTargetPose(-3,0,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.isCurrStalled() || timeUp(startTime,5000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(0,0,0);
        }
        break;

        case 5:
        //run towards final button
        drivetrain.setTargetPose(5,-10,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.isCurrStalled() || timeUp(startTime,5000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.setPose(0,0,0);
            drivetrain.stop();
        }
        break;

        case 6:
        //turn to hit button
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1050)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  -90 deg
            drivetrain.setPose(0,0,-90);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
        }
        break;
       

        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool ReturnToStart::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void ReturnToStart::stop(){end = true;}

//returns path name
std::string ReturnToStart::getName(){return commandName;}