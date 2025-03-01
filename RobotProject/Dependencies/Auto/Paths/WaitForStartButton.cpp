#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "WaitForStartButton.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
WaitForStartButton::WaitForStartButton(HolonomicTriangleDrive &dt) : drivetrain(dt){
    

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void WaitForStartButton::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void WaitForStartButton::run(){
    
    //end condition
    if(timeUp(startTime,6000)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool WaitForStartButton::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void WaitForStartButton::stop(){end = true;}

//returns path name
std::string WaitForStartButton::getName(){return commandName;}