#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PathTest.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
PathTest::PathTest(HolonomicTriangleDrive &dt) : drivetrain(dt){
    

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void PathTest::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void PathTest::run(){
    int totalDuration = 6800;
    //Command stuff

    //basically drive forward for 6000 milisec and drift right slightly

    drivetrain.setMovementVector(0.09,0.5,0);
    //stop moving to right halfway through command
    if(timeUp(startTime,totalDuration/2)){
        drivetrain.setMovementVector(0,0.3,0);
    }
    drivetrain.update();

    LCD.WriteAt("PathTest Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    //end condition
    if(timeUp(startTime,totalDuration)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool PathTest::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PathTest::stop(){end = true;}

//returns path name
std::string PathTest::getName(){return commandName;}