#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "TurnRight30Deg.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
TurnRight30Deg::TurnRight30Deg(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void TurnRight30Deg::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void TurnRight30Deg::run(){
    //Command stuff

    //Forwards 5s
    drivetrain.setMovementVector(0,0,0.5);
    drivetrain.update();

    LCD.WriteAt("TurnRight30Deg Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    //end condition
    if(timeUp(startTime,2500)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool TurnRight30Deg::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void TurnRight30Deg::stop(){end = true;}

//returns path name
std::string TurnRight30Deg::getName(){return commandName;}