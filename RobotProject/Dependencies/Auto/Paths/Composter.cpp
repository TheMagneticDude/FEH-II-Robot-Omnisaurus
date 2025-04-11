#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Composter.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Composter::Composter(HolonomicTriangleDrive &dt, FEHServo &a, FEHMotor &ca) : drivetrain(dt), arm(a), composterArm(ca){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void Composter::init(){
    //starting circle is 0,0 with -x being sideways towards composter and +y being forwards towards ramp
    //robot starts at a -45 degree angle  
    drivetrain.resetMotorCounts();
    
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(0,0,-45);
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void Composter::run(){
    
    //Command stuff
    std::string s =  "Comp SubPath: " + std::to_string(i);
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
        arm.SetDegree(180);
        //turn right 45 so back is straight with wall (so y+ is up the ramp)
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,400)){
            drivetrain.setPose(0,0,0);
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        

        case 1:
        drivetrain.setTargetPose(-3,0,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        //move back to aling with wall
        drivetrain.setTargetPose(-2,-5,0);
        drivetrain.runToPoseLim(0.6);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1000)){
            // drivetrain.setMovementVector(0,0,0);
            //should now be aligned at y = 0
            drivetrain.setPose(drivetrain.getPose()[0], 0, 0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        //move forward 0.5 in to have room to rotate
        drivetrain.setTargetPose(-2,1,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1350)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  150 deg
            drivetrain.setPose(-2,1,-150);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        //move back towards wall
        drivetrain.setTargetPose(-2,0,-150);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,500)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 6:
        //move towards composter
        drivetrain.setTargetPose(-6,0,-150);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos()){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 7:
        composterArm.SetPercent(80);
        if(timeUp(startTime,1500)){
            startTime = TimeNowMSec();
            i++; 
        composterArm.SetPercent(0);
        }
        break;

        case 8:
        composterArm.SetPercent(-80);
        if(timeUp(startTime,1500)){
            startTime = TimeNowMSec();
            i++; 
        composterArm.SetPercent(0);
        drivetrain.setPose(-6,0,-150);
        }
        break;

        case 9:
        //move away from composter
        drivetrain.setTargetPose(-3,0,-150);
        drivetrain.runToPoseLim(0.4);
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
bool Composter::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void Composter::stop(){end = true;}

//returns path name
std::string Composter::getName(){return commandName;}