#include <limits>
#include <string>
#include <FEHLCD.h>
#include "Motor.h"

using namespace std;



Motor::Motor(FEHMotor::FEHMotorPort p, float maxvolt, FEHIO::FEHIOPin encoderPort, float countsperrev) : M(p,maxvolt), MotorEncoder(encoderPort){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = countsperrev;
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin encoderPort, float countsperrev) : M(p,defaultMotorMaxVolt), MotorEncoder(encoderPort){
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

void Motor::runToPosition(){
    float target = targetPos;
    currPosition = getCounts();

    float delta = targetPos - currPosition;

    float power = clamp(delta,-maxSpeed,maxSpeed);

    M.SetPercent(power);
}

void Motor::runToPosition(float pos){
    targetPos = pos;
    runToPosition();
}
void Motor::resetEncoderCounts(){
    MotorEncoder.ResetCounts();
}
float Motor::getCounts(){
    return MotorEncoder.Counts();
}
void Motor::runAtVelocity(float v){
    
}
