#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PathTest2.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
PathTest2::PathTest2(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void PathTest2::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void PathTest2::run(){
    //Command stuff

    //basically drive to the side and somewhat forwards for 2200 milisec

    drivetrain.setMovementVector(-0.08,0.52,0);
    drivetrain.update();

    LCD.WriteAt("PathTest2 Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    //end condition
    if(timeUp(startTime,3000)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool PathTest2::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PathTest2::stop(){end = true;}

//returns path name
std::string PathTest2::getName(){return commandName;}