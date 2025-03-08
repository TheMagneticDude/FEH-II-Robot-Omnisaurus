#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "SelectButton.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
SelectButton::SelectButton(HolonomicTriangleDrive &dt, AnalogInputPin &CDS) : drivetrain(dt), CDS(CDS){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    didInit = false;
    CDSisRed = false;//defaults to blue
}

void SelectButton::init(){
    startTime = TimeNowMSec();
}

//Runs the command every tick
void SelectButton::run(){
    //Command telemetry
    LCD.WriteAt("SelectButton Running...",0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);

    float CDS_None = 1;//0.8
    float CDS_Red = 2.9;//3.18
    float CDS_Blue = 2.6;//2.8

    if(!didInit){
        //if button is red
        if(CDS.Value() > CDS_Red){
            CDSisRed = true;
            didInit = true;
            startTime = TimeNowMSec();
        }else if (CDS.Value() > CDS_Blue && CDS.Value() < CDS_Red){
            CDSisRed = false;
            didInit = true;
            startTime = TimeNowMSec();
            //reset start time timer for path once color is determined
        }
        if(timeUp(startTime,1500)){
            //timeout if robot cant read button light
            didInit = true;
            startTime = TimeNowMSec();
            //defaults to blue button path
        }
    }else{
        //once init has happened
        if(CDSisRed){
            //red path
            LCD.Clear();
            LCD.SetBackgroundColor(RED);
            drivetrain.setMovementVector(0.06,0.2,0);
            drivetrain.update();
        }else{
            //blue path
            LCD.Clear();
            LCD.SetBackgroundColor(BLUE);
            drivetrain.setMovementVector(-0.06,0.2,0);
            drivetrain.update();
        }

        if(timeUp(startTime,2300)){
            end = true;
            drivetrain.stop();
        }
        
    }

    
    




    
    
}

//exit condition, returns true once command sequence has ended
bool SelectButton::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void SelectButton::stop(){end = true;}

//returns path name
std::string SelectButton::getName(){return commandName;}