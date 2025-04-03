#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Composter.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Composter::Composter(HolonomicTriangleDrive &dt, FEHServo &a) : drivetrain(dt), arm(a){

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
    LCD.WriteAt(drivetrain.getPose()[1],0,60);


    
    switch(i){
        case 0:
        //path 1
        // arm.SetDegree(180);
        //forward 1 in
        drivetrain.setTargetPose(0,1,0);
        drivetrain.runToPose();
        if(drivetrain.getReachedTargetPos()){
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