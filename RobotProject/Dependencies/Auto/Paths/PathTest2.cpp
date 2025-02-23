#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PathTest2.h"


using namespace std;

//take in the drivetrain object and any subsystems needed for path
PathTest2::PathTest2(HolonomicTriangleDrive &dt) : drivetrain(dt){
    //save start timepoint
    start = std::chrono::steady_clock::now();
    //init end flag
    end = false;
}

//Runs the command every tick
void PathTest2::run(){
    //Command stuff

    //basically drive backwards for 3000 milisec
    drivetrain.setMovementVector(0,-0.8,0);
    drivetrain.update();


    //end condition
    if(timeUp(start,chronMiliSec(3000))){
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool PathTest2::ended(){return end;}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PathTest2::stop(){end = true;}

//returns path name
std::string PathTest2::getName(){return commandName;}