#include <limits>
#include <string>
#include <FEHLCD.h>
#include "Motor.h"
#include "../Dependencies/Assert.h"
#include "../Dependencies/VelocityPID.h"



using namespace std;


Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt) : M(p,maxvolt), MotorEncoder(ep),velocityPID(0, 0){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = defaultCountsPerRev;
    encoderPort = encoderPort;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;
    motorDirection = Direction::Idle;
    lastEncoderCount = 0;
    lastTime = TimeNowMSec();
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt, float countsperrev) : M(p,maxvolt), MotorEncoder(ep),velocityPID(0, 0){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = countsperrev;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;
    lastEncoderCount = 0;
    lastTime = TimeNowMSec();
}

void Motor::setMode(Mode m){
    motorMode = m;
}

void Motor::SetPercent(float percent){
    //Power is used by both velocity and run to position
    (percent > 0) ? motorDirection = Direction::FORWARD : motorDirection = Direction::BACKWARD;
    if(percent ==0){motorDirection = Direction::Idle;}
    M.SetPercent(percent);
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

        (power > 0) ? motorDirection = Direction::FORWARD : motorDirection = Direction::BACKWARD;
        if(power ==0){motorDirection = Direction::Idle;}
        M.SetPercent(power);
    }else{
        assertError("Motor is not in RUN_TO_POSITION mode");
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
    lastEncoderCount = 0;
    lastTime = TimeNowMSec();

    float currTime = TimeNowMSec();
    float currCount = MotorEncoder.Counts();

    float deltaCounts = currCount - lastEncoderCount;
    float deltaTime = (currTime - lastTime); //in milisecs
    if(deltaTime <= 0){
        //no divide by zero error
        return 0;
    }
    float rotations = deltaCounts / encoderCountsPerRev;
    
    float distance = rotations * wheelCircumference;

    float deltaTimeSec = deltaTime / 1000.0;

    float velocity = distance / deltaTimeSec;

    if(motorDirection == Direction::BACKWARD){
        velocity = -velocity;
    }


    //update values for next loop
    lastEncoderCount = currCount;
    lastTime = currTime;

    return velocity;
}
void Motor::runAtVelocity(float v){
    if(motorMode == Mode::VELOCITY){
        targetVelocity = v;
        (v > 0) ? motorDirection = Direction::FORWARD : motorDirection = Direction::BACKWARD;
        if(v ==0){motorDirection = Direction::Idle;}

        float currentVelocity = getVelocity();

        //get PID calculation
        int pidOutput = velocityPID.pidCalc(targetVelocity,currentVelocity);

        //convert pid output to motor percentage
        float motorPower = (pidOutput/maxSpeed) * 100.0;
        motorPower = clamp(motorPower, -100,100);//clamp percentage between -100% and 100%

        SetPercent(motorPower);
    }else{
        assertError("Motor is not in VELOCITY mode");
    }
    
}







  
