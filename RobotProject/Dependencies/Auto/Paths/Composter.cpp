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
    std::string s = "Composter SubPath: " + i;
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);
    LCD.WriteAt(drivetrain.getPose()[0],0,60);
    LCD.WriteAt(drivetrain.getPose()[1],0,75);
    LCD.WriteAt(drivetrain.getPose()[2],0,90);


    
    switch(i){
        case 0:
        arm.SetDegree(180);
        //turn left 120 so straight with wall
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1600)){
            drivetrain.setMovementVector(0,0,0);
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 1:
        drivetrain.setMovementVector(0.3,0.2,0);
        drivetrain.update();
        if(timeUp(startTime,1600)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 2:
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,500)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 3:
        drivetrain.setMovementVector(0.3,0.3,0);
        drivetrain.update();
        if(timeUp(startTime,800)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        case 4:
        composterArm.SetPercent(80);
        if(timeUp(startTime,800)){
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