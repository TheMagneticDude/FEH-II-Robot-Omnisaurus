#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PoseTest.h"
#include <cstring>


using namespace std;


//take in the drivetrain object and any subsystems needed for path
PoseTest::PoseTest(HolonomicTriangleDrive &dt) : drivetrain(dt){
    //save start timepoint
    startTime = TimeNowMSec();
    //init end flag
    end = false;
    i = 0;
}

void PoseTest::init(){
    drivetrain.setPose(0,0,0);
    startTime = TimeNowMSec();
    i = 0;
    drivetrain.resetMotorCounts();
}

//Runs the command every tick
void PoseTest::run(){
    
    //Command stuff
    std::string s = "PoseTest SubPath: " + std::to_string(i);
    LCD.WriteAt(s.c_str(),0,0);

    auto elapsed = TimeNowMSec() - startTime;
    std::string elapsedS = "Elapsed: " + std::to_string(elapsed) + " ms";
    // LCD.WriteAt("Elapsed: ",0,15);
    LCD.WriteAt(elapsedS.c_str(),0,30);
    // LCD.WriteAt("ms",0,45);
    std::string pose = "Pose: [" + std::to_string(drivetrain.getPose()[0]) + ", " + std::to_string(drivetrain.getPose()[1]) + std::to_string(drivetrain.getPose()[2]) + "]";
    LCD.WriteAt(pose,0,60);



    
    switch(i){
        case 0:

        break;
        

        default:
        drivetrain.stop();
        end = true;
        break;
    }
}

//exit condition, returns true once command sequence has ended
bool PoseTest::ended(){
    return end;
}

//Stops the command even if end condition has not been reached and triggers ended to move to next command in sequence
void PoseTest::stop(){end = true;}

//returns path name
std::string PoseTest::getName(){return commandName;}