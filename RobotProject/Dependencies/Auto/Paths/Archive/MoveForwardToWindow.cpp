#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "MoveForwardToWindow.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
MoveForwardToWindow::MoveForwardToWindow(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void MoveForwardToWindow::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void MoveForwardToWindow::run(){
    //Command stuff

    //Forwards 5s
    drivetrain.setMovementVector(0,0.8,0);
    drivetrain.update();

    LCD.WriteAt("MoveForwardToWindow Running...",0,0);

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
bool MoveForwardToWindow::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void MoveForwardToWindow::stop(){end = true;}

//returns path name
std::string MoveForwardToWindow::getName(){return commandName;}