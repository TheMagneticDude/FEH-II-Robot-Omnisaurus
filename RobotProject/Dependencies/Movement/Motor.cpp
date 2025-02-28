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
    velocityLoopTimerPass = false;
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
    velocityLoopTimerPass = false;
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
        assertError("Assertion Error: Motor is not in RUN_TO_POSITION mode");
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
//get velocity in inch per second
float Motor::getVelocity(){
    float velocityLoopTimerMs = 1;//loop time of 1ms    

    float currTime = TimeNowMSec();
    

    if(!velocityLoopTimerPass){
        lastTime = currTime;
        velocityLoopTimerPass = true;
        lastEncoderCount = MotorEncoder.Counts();
        return currentVelocity;//prevent issues on first call
    }
    
    float deltaTime = (currTime - lastTime); //in milisecs

    if(deltaTime < velocityLoopTimerMs){
        return currentVelocity;
    }
    float currCount = MotorEncoder.Counts();

    float deltaCounts = currCount - lastEncoderCount;
        
    if(deltaTime <= 0){
        //no divide by zero error
        return currentVelocity;
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

    currentVelocity = velocity;
        
    // reset the timer pass so it waits for another interval
    velocityLoopTimerPass = false;

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
        assertError("Assertion Error: Motor is not in VELOCITY mode");
    }
    
}

void Motor::setPID(float P, float I, float D){
    //set constants for PID loop for this specific motor
    velocityPID.setPID(P,I,D);
}







  
