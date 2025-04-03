#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "AppleBasket.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
AppleBasket::AppleBasket(HolonomicTriangleDrive &dt, FEHServo &a) : drivetrain(dt), arm(a){

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void AppleBasket::init(){
    startTime = TimeNowMSec();
    i = 0;
}

//Runs the command every tick
void AppleBasket::run(){
    
    //Command stuff
    std::string s = "AppleBasket SubPath: " + i;
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);


    
    switch(i){
        case 0:
        //path 1
        arm.SetDegree(180);
        //forward 1 in
        drivetrain.setMovementVector(0,1,0);
        drivetrain.update();
        if(timeUp(startTime,1500)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 1:
        //turn 30
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,500)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 2:
        arm.SetDegree(120);
        drivetrain.setMovementVector(-0.3,0,0);
        drivetrain.update();
        if(timeUp(startTime,2000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 3:
        arm.SetDegree(90);
        drivetrain.setMovementVector(0.3,0,0);
        drivetrain.update();
        if(timeUp(startTime,2000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 4:
        arm.SetDegree(60);
        drivetrain.setMovementVector(0.6,-0.3,0);
        drivetrain.update();
        if(timeUp(startTime,2000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 5:
        //go up ramp
        arm.SetDegree(60);
        drivetrain.setMovementVector(0,0.3,0);
        drivetrain.update();
        if(timeUp(startTime,8000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 6:
        //turn 90 left
        drivetrain.setMovementVector(0,0,-0.5);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 7:
        drivetrain.setMovementVector(0.3,-0.3,0);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            i++; 
        }
        break;
        case 8:
        drivetrain.setMovementVector(0,-0.3,0);
        drivetrain.update();
        if(timeUp(startTime,500)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 9:
        drivetrain.setMovementVector(0.3,0.5,0);
        drivetrain.update();
        if(timeUp(startTime,3000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;

        case 10:
        arm.SetDegree(40); 
        drivetrain.setMovementVector(0.3,0,0);
        drivetrain.update();
        if(timeUp(startTime,2000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        
        case 11:
        
        drivetrain.setMovementVector(0,0,0.5);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;
        case 12:
        arm.SetDegree(160);
        drivetrain.setMovementVector(0,0.2,0);
        drivetrain.update();
        if(timeUp(startTime,1000)){
            startTime = TimeNowMSec();
            i++;
        }
        break;


        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool AppleBasket::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void AppleBasket::stop(){end = true;}

//returns path name
std::string AppleBasket::getName(){return commandName;}