#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PathTest.h"


using namespace std;

//take in the drivetrain object and any subsystems needed for path
PathTest::PathTest(HolonomicTriangleDrive &dt) : drivetrain(dt){
    //save start timepoint
    start = std::chrono::steady_clock::now();
    //init end flag
    end = false;
}

//Runs the command every tick
void PathTest::run(){
    //Command stuff

    //basically drive forward for 3000 milisec
    drivetrain.setMovementVector(0,0.8,0);
    drivetrain.update();


    //end condition
    if(timeUp(start,chronMiliSec(3000))){
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool PathTest::ended(){return end;}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PathTest::stop(){end = true;}