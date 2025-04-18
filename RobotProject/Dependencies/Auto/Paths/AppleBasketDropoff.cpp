#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AppleBasketDropoff.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AppleBasketDropoff::AppleBasketDropoff(HolonomicTriangleDrive &dt, FEHServo &a) : drivetrain(dt), arm(a){

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
    drivetrain.toggleVelocityControl(true);
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
        drivetrain.runToPoseLim(0.8);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
        }
        break;

        case 2:
        //align to wall next to ramp
        drivetrain.setTargetPose(35,5,-90);
        drivetrain.toggleVelocityControl(false);//so it can stall
        drivetrain.runTilStalled(0.5);
        if(drivetrain.isCurrStalled() || timeUp(startTime,3000)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,5,-90);
            startTime = TimeNowMSec();
            i++;
            drivetrain.resetStallDetection();
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 3:
        //move -x some to rotate
        drivetrain.setTargetPose(-1,5,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        //turn towards ramp (to 0 deg)
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,1050)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  0 deg
            drivetrain.setPose(-1,5,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 5:
        //move up ramp
        drivetrain.setTargetPose(2,60,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,6500)){
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
            drivetrain.resetStallDetection();
        }
        break;

        case 7:
        //align x
        drivetrain.toggleVelocityControl(false);
        drivetrain.setTargetPose(25,25,-90);
        drivetrain.runTilStalled(0.4);
        if(drivetrain.isCurrStalled() || timeUp(startTime,1500)){
            // drivetrain.setMovementVector(0,0,0);
            drivetrain.setPose(0,25,-90);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.resetStallDetection();
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 8:
        drivetrain.toggleVelocityControl(true);
        //forward one to avoid bumping into acryllic 
        drivetrain.setTargetPose(-1,25,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++;
        }
        break;

        case 9:
        //align x and y
        drivetrain.setTargetPose(5,35,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,3000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            drivetrain.setPose(0,0,-90);//0,0 set in corner of apple basket top drawer
            i++; 
        }
        break;

        case 10:
        //move away to turn
        drivetrain.setTargetPose(-1,-1,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 11:
        //turn towards apple basket to drop off (to 90 deg)
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,2050)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  90 deg
            drivetrain.setPose(0,0,90);
            startTime = TimeNowMSec();
            i++; 
        }
        break;


        case 12:
        //Move towards wall to align front wheel
        drivetrain.setTargetPose(5,1,90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            startTime = TimeNowMSec();
            drivetrain.setPose(0,1,90);
            i++; 
        }
        break;

        case 13:
        //Move away from wall to move towards apple basket deposit
        drivetrain.setTargetPose(-0.1,1,90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,800)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 14:
        //Move towards cabinet to deposit basket
        drivetrain.setTargetPose(-0.1,3,90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            drivetrain.setPose(-0.1,0,90);
            drivetrain.stop();
            i++; 
        }
        break;

        case 15:
        //lower arm
        arm.SetDegree(90);//need to change angle to dropoff
        if(timeUp(startTime,500)){
            drivetrain.setMovementVector(0,0,0);
            drivetrain.stop();
            startTime = TimeNowMSec();
            // drivetrain.setPose(0,0,-90);
            i++; 
        }
        break;

        case 16:
        //Move back away from apple basket
        drivetrain.setTargetPose(-0.1,-1,90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 17:
        //Raise arm again
        arm.SetDegree(0);
        if(timeUp(startTime,500)){
            drivetrain.setMovementVector(0,0,0);
            drivetrain.stop();
            startTime = TimeNowMSec();
            // drivetrain.setPose(0,0,-90);
            i++; 
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 18:
        //turn for window (to -90 deg)
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,2050)){
            drivetrain.setMovementVector(0,0,0);
            //robot SHOULD now be at  -90 deg
            drivetrain.setPose(0,0,-90);
            startTime = TimeNowMSec();
            i++; 
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 19:
        //Move towards wall to align again
        drivetrain.setTargetPose(5,-1,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1800)){
            startTime = TimeNowMSec();
            drivetrain.setPose(0,-0.1,-90);
            i++;
            drivetrain.toggleVelocityControl(true);
        }
        break;

        case 20:
        drivetrain.toggleVelocityControl(true);
        //align x and y
        drivetrain.setTargetPose(10,35,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,900)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            drivetrain.setPose(0,0,-90);//0,0 set in corner of apple basket top drawer
            i++; 
        }
        break;

        case 21:
        //Move towards wall to align again
        drivetrain.setTargetPose(5,0,-90);
        drivetrain.runToPoseLim(0.2);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,500)){
            startTime = TimeNowMSec();
            drivetrain.setPose(0,-0.1,-90);
            i++;
        }
        break;

        case 22:
        //move away
        drivetrain.setTargetPose(0,-1,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,1000)){
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
bool AppleBasketDropoff::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AppleBasketDropoff::stop(){end = true;}

//returns path name
std::string AppleBasketDropoff::getName(){return commandName;}