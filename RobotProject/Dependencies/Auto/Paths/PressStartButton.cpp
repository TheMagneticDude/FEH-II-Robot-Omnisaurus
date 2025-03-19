#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PressStartButton.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
PressStartButton::PressStartButton(HolonomicTriangleDrive &dt) : drivetrain(dt){
    

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void PressStartButton::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void PressStartButton::run(){
    int totalDuration = 1000;
    //Command stuff

    //basically drive back slightly

    drivetrain.setMovementVector(0,-0.3,0);
    drivetrain.update();

    LCD.WriteAt("PressStartButton Running...",0,0);

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
bool PressStartButton::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PressStartButton::stop(){end = true;}

//returns path name
std::string PressStartButton::getName(){return commandName;}