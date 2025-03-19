#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AlignInCorner.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AlignInCorner::AlignInCorner(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void AlignInCorner::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void AlignInCorner::run(){
    int totalDuration = 3300;
    //Command stuff

    drivetrain.setMovementVector(0.55,-0.33,0);
    //stop moving to left halfway through command
    if(timeUp(startTime,totalDuration*2/3)){
        drivetrain.setMovementVector(0,-0.2,0);
    }
    drivetrain.update();

    LCD.WriteAt("AlignInCorner Running...",0,0);

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
bool AlignInCorner::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AlignInCorner::stop(){end = true;}

//returns path name
std::string AlignInCorner::getName(){return commandName;}