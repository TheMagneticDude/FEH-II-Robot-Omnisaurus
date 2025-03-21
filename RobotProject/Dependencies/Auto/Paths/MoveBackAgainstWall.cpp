#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "MoveBackAgainstWall.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
MoveBackAgainstWall::MoveBackAgainstWall(HolonomicTriangleDrive &dt) : drivetrain(dt){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
}

void MoveBackAgainstWall::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void MoveBackAgainstWall::run(){
    //Command stuff
    LCD.WriteAt("MoveBackAgainstWall Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    if(!timeUp(startTime,1000)){
        //back
        drivetrain.setMovementVector(0,-0.5,0);
        drivetrain.update();
    }else{
        drivetrain.setMovementVector(-0.5,0,0);
        drivetrain.update();
    }

    //end condition
    if(timeUp(startTime,5000)){
        drivetrain.stop();
        end = true;
    }
}

//exit condition, returns true once command sequence has ended
bool MoveBackAgainstWall::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void MoveBackAgainstWall::stop(){end = true;}

//returns path name
std::string MoveBackAgainstWall::getName(){return commandName;}