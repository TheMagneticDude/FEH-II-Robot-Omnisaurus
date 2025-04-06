#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Composter.h"
#include <cstring>


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
    drivetrain.setPose(0,0,0);
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void Composter::run(){
    
    //Command stuff
    std::string s = "Composter SubPath: " + std::to_string(i);
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = "Elapsed: " + std::to_string(elapsed) + " ms";
    // LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    // LCD.WriteAt("ms",0,45);
    std::string pose = "Pose: [" + std::to_string(drivetrain.getPose()[0]) + ", " + std::to_string(drivetrain.getPose()[1]) + std::to_string(drivetrain.getPose()[2]) + "]";
    LCD.WriteAt(pose,0,60);



    
    switch(i){
        case 0:
        arm.SetDegree(180);
        //turn left 120 so straight with wall
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1200)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        drivetrain.setMovementVector(0.3,0.4,0);
        drivetrain.update();
        if(timeUp(startTime,1600)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        drivetrain.setMovementVector(0.3,0.3,0);
        drivetrain.update();
        if(timeUp(startTime,800)){
            drivetrain.setMovementVector(0,0,0);
            drivetrain.stop();
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        drivetrain.setMovementVector(-0.2,-0.2,0);
        drivetrain.update();
        if(timeUp(startTime,100)){
            drivetrain.setMovementVector(0,0,0);
            drivetrain.stop();
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        

        case 4:
        composterArm.SetPercent(80);
        if(timeUp(startTime,1500)){
            startTime = TimeNowMSec();
            i++; 
        composterArm.SetPercent(0);
        }
        break;

        case 5:
        composterArm.SetPercent(-80);
        if(timeUp(startTime,1500)){
            startTime = TimeNowMSec();
            i++; 
        composterArm.SetPercent(0);
        }
        break;

        case 6:
        drivetrain.setMovementVector(-0.3,-0.3,0);
        drivetrain.update();
        arm.SetDegree(0);
        if(timeUp(startTime,2000)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 7:
        drivetrain.setMovementVector(-0.5,0,0);
        drivetrain.update();
        if(timeUp(startTime,8000)){
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