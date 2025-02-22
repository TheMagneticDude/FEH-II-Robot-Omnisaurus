#include <limits>
#include <string>
#include <FEHLCD.h>
#include "Motor.h"

using namespace std;


Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt) : M(p,maxvolt), MotorEncoder(ep){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = defaultCountsPerRev;
    encoderPort = encoderPort;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt, float countsperrev) : M(p,maxvolt), MotorEncoder(ep){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = countsperrev;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;
}

void Motor::setMode(Mode m){
    motorMode = m;
}

void Motor::SetPercent(float percent){
    if(motorMode == Mode::POWER){
        M.SetPercent(percent);
    }else{
        LCD.WriteLine("Motor is not in POWER mode");
    }
}

void Motor::Stop(){
    M.Stop();
}

void Motor::runToPosition(){
    if(motorMode == Mode::RUN_TO_POSITION){
        float target = targetPos;
        currPosition = getCounts();

        float delta = targetPos - currPosition;

        float power = clamp(delta,-maxSpeed,maxSpeed);

        M.SetPercent(power);
    }else{
        LCD.WriteLine("Motor is not in RUN_TO_POSITION mode");
    }
}

void Motor::setTargetPos(float pos){
    targetPos = pos;
}
void Motor::resetEncoderCounts(){
    MotorEncoder.ResetCounts();
}
float Motor::getCounts(){
    return MotorEncoder.Counts();
}
float Motor::getVelocity(){
    return 0;
}
void Motor::runAtVelocity(float v){
    if(motorMode == Mode::VELOCITY){
        
    }else{
        LCD.WriteLine("Motor is not in VELOCITY mode");
    }
    
}
