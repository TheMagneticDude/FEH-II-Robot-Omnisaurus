#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Buttons.h"
#include <cstring>
#include <iomanip>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Buttons::Buttons(HolonomicTriangleDrive &dt, AnalogInputPin cds) : drivetrain(dt), CDS(cds){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void Buttons::init(){
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.setPose(0,0,-90);
    drivetrain.resetMotorCounts();

    //init end flag
    end = false;
    didInit = false;
    CDSisRed = false;//defaults to blue
}

//Runs the command every tick
void Buttons::run(){
    
    //Command stuff
    std::string s = "Buttons SubPath: " + i;
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

    float CDS_None = 1;//0.8
    float CDS_Red = 2.9;//3.18
    float CDS_Blue = 2.6;//2.8
    
    switch(i){

        case 0:
        //move towards button
        drivetrain.setTargetPose(-6,-1,-90);
        drivetrain.runToPoseLim(0.4);
        if(drivetrain.getReachedTargetPos() || timeUp(startTime,2000)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        //scan CDS
        //if button is red
        if(CDS.Value() > CDS_Red){
            CDSisRed = true;
            didInit = true;
            startTime = TimeNowMSec();
        }else if (CDS.Value() > CDS_Blue && CDS.Value() < CDS_Red){
            CDSisRed = false;
            didInit = true;
            startTime = TimeNowMSec();
            //reset start time timer for path once color is determined
        }

        //exit if takes too long
        if(didInit || timeUp(startTime,1500)){
            // drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        if(CDSisRed){
            drivetrain.setTargetPose(-8,0,-90);
            drivetrain.runToPoseLim(0.4);
        }else{
            drivetrain.setTargetPose(-8,-2,-90);
            drivetrain.runToPoseLim(0.4);
        }


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
bool Buttons::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void Buttons::stop(){end = true;}

//returns path name
std::string Buttons::getName(){return commandName;}