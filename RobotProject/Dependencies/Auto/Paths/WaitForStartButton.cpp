#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "WaitForStartButton.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
WaitForStartButton::WaitForStartButton(AnalogInputPin &CDS) : CDS(CDS){
    

    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;

    std::string nameRunning = commandName + " Running...";
    LCD.WriteAt(nameRunning,0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = std::to_string(elapsed);
    LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    LCD.WriteAt("ms",0,45);
    
}

void WaitForStartButton::init(){
    startTime = TimeNowMSec();
    CDSTriggered = false;
}

//Runs the command every tick
void WaitForStartButton::run(){
    const float CDS_Red = 2;//need to change to threshold for CDS cell red light
    if(CDS.Value() >= CDS_Red){
        CDSTriggered = true;
    }
    //end condition
    if(CDSTriggered){
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