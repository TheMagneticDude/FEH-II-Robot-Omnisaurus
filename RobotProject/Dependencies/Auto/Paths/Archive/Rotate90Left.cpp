#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "Rotate90Left.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
Rotate90Left::Rotate90Left(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void Rotate90Left::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void Rotate90Left::run(){
    //Command stuff

    //Rotate 90 deg left

    drivetrain.setMovementVector(0,0,-0.5);
    drivetrain.update();

    LCD.WriteAt("Rotate90Left Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    //end condition
    if(timeUp(startTime,1000)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool Rotate90Left::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void Rotate90Left::stop(){end = true;}

//returns path name
std::string Rotate90Left::getName(){return commandName;}