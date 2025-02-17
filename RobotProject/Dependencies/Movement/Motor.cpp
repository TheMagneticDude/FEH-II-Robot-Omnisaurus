#include "HolonomicTriangleDrive.h"
#include <limits>
#include <string>
#include <FEHLCD.h>
#include <Motor.h>

using namespace std;


Motor::Motor(FEHMotor::FEHMotorPort p, float maxvolt, FEHIO::FEHIOPin encoderPort, float countsperrev) : M(p,maxvolt), encoder(encoderPort){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = countsperrev;
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin encoderPort, float countsperrev) : M(p,defaultMotorMaxVolt), encoder(encoderPort){
    port = p;
    MotorMaxVolt = defaultMotorMaxVolt;
    encoderCountsPerRev = countsperrev;
}

void Motor::SetPercent(float percent){
    M.SetPercent(percent);
}

void Motor::Stop(){
    M.Stop();
}

void runToPosition(){

}
void resetEncoderCounts(){

}
float getCounts(){

}
void runAtVelocity(float v){

}
