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
    targetVelocity = 0;
    pidOut=0;

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
    targetVelocity = 0;
    pidOut=0;

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
float Motor::getTargetVelocity(){
    return targetVelocity;
}
float Motor::getPIDOut(){
    return pidOut;
}
//get velocity in inch per second
float Motor::getVelocity(){
    float velocityEpsilon = 1; //Min amount of encoder delta to update velocity
    float velocityLoopTimerMs = 1;//max loop time of 1ms    

    float currTime = TimeNowMSec();
    float deltaTime = (currTime - lastTime); //in milisecs
    float currCount = MotorEncoder.Counts();

    float deltaCounts = currCount - lastEncoderCount;
    if (motorDirection == Direction::BACKWARD) {
        deltaCounts = -deltaCounts;
    }
    

    if(!velocityLoopTimerPass){
        lastTime = currTime;
        velocityLoopTimerPass = true;
        lastEncoderCount = currCount;
    }
    
    
    //wait until count has changed enough by at least epsilon, or it has not changed enough in 1ms and remeasure
    if((fabs(deltaCounts) < velocityEpsilon) && (deltaTime <= velocityLoopTimerMs)){
        return currentVelocity;
    }

        
    if(deltaTime <= 0){
        //no divide by zero error
        return currentVelocity;
    }


    //prevent float error issues
    if (deltaCounts == 0) {
        currentVelocity = 0;
        return 0;
    }

    

    float rotations = deltaCounts / encoderCountsPerRev;
        
    float distance = rotations * wheelCircumference;

    float deltaTimeSec = deltaTime / 1000.0;

    float velocity = distance / deltaTimeSec;

    // if(motorDirection == Direction::BACKWARD){
    //     velocity = -velocity;
    // }


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
        // (v > 0) ? motorDirection = Direction::FORWARD : motorDirection = Direction::BACKWARD;
        // if(v ==0){motorDirection = Direction::Idle;}

        float currentVelocity = getVelocity();
        float currTime = TimeNowMSec();
        float loopTime = (currTime - lastTime);

        if(loopTime <= 0){
            loopTime = 0.001; //assume smol time if no time passed
        }

        //get PID calculation
        float pidOutput = velocityPID.pidCalcLoopTime(targetVelocity,currentVelocity,loopTime);
        pidOut = pidOutput;
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







  
