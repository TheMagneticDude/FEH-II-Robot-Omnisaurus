#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "PoseTest.h"
#include <cstring>
#include <iomanip>


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
    //starting circle is 0,0 with -x being sideways towards composter and +y being forwards towards ramp
    //robot starts at a -45 degree angle  
    drivetrain.resetMotorCounts();
    drivetrain.setPose(0,0,-45);
    startTime = TimeNowMSec();
    i = 0;
    
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
    stringstream x;
    x << std::fixed << std::setprecision(2) << drivetrain.getPose()[0];
    
    stringstream y;
    y << std::fixed << std::setprecision(2) << drivetrain.getPose()[1];
    
    stringstream theta;
    theta << std::fixed << std::setprecision(2) << drivetrain.getPose()[2];

    std::string pose = "Pose: [" + x.str() + ", " + y.str() + ", " + theta.str() + "]";
    LCD.WriteAt(pose,0,60);



    
    switch(i){
        case 0:
        //rotate to face composter
        drivetrain.setTargetPose(0,0,-90);
        drivetrain.runToPose();

        if(drivetrain.getReachedTargetPos()){
            startTime = TimeNowMSec();
            i++; 
        }
        break;

        // case 1:
        // //move towards composter (-5 in away) still rotated - 90
        // drivetrain.setTargetPose(-5,0,-90);
        // drivetrain.runToPose();

        // if(drivetrain.getReachedTargetPos()){
        //     startTime = TimeNowMSec();
        //     i++; 
        // }
        // break;

        // case 2:
        // //move back to starting pos but this time both rotate and move at the same time
        // //this is the true power of pose estimation
        // drivetrain.setTargetPose(0,0,0);
        // drivetrain.runToPose();

        // if(drivetrain.getReachedTargetPos()){
        //     startTime = TimeNowMSec();
        //     i++; 
        // }
        // break;

        

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