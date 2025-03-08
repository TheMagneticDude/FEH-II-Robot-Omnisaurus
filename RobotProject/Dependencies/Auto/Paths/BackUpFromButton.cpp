#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "BackUpFromButton.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
BackUpFromButton::BackUpFromButton(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void BackUpFromButton::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void BackUpFromButton::run(){
    //Command stuff

    //basically drive to the side and somewhat forwards for 2200 milisec

    drivetrain.setMovementVector(0,-0.5,0);
    drivetrain.update();

    LCD.WriteAt("BackUpFromButton Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    //end condition
    if(timeUp(startTime,3500)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool BackUpFromButton::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void BackUpFromButton::stop(){end = true;}

//returns path name
std::string BackUpFromButton::getName(){return commandName;}