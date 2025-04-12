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
    encoderPort = ep;
    MotorEncoder.ResetCounts();
    targetVelocity = 0;
    pidOut=0;
    totalDisplacement = 0;
    velocityDeltaTime = 0;
    currentVelocity = 0;

    //default mode is power
    motorMode = Mode::POWER;
    motorDirection = Direction::Idle;
    lastEncoderCount = 0;
    lastTime = TimeNowMSec();
    velocityLoopTimerPass = false;
    currPosition = 0;
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt, float countsperrev) : M(p,maxvolt), MotorEncoder(ep),velocityPID(0, 0){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderPort = ep;
    encoderCountsPerRev = countsperrev;
    MotorEncoder.ResetCounts();
    targetVelocity = 0;
    pidOut=0;
    totalDisplacement = 0;
    velocityDeltaTime = 0;
    currentVelocity = 0;

    //default mode is power
    motorMode = Mode::POWER;
    lastEncoderCount = 0;
    lastTime = TimeNowMSec();
    velocityLoopTimerPass = false;
    currPosition = 0;
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

        float power = clamp(delta,-100,100);

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
    lastEncoderCount = 0;
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
float Motor::getTotalDisplacement(){
    return totalDisplacement;
}
//get velocity in inch per second
float Motor::getVelocity(){
    float velocityEpsilon = 1;

    float velocityLoopTimerMsMin = 5;//min loop time of 5 ms  
    float velocityLoopTimerMsMax = 50;//max loop time of 50 ms  
    

    float currTime = TimeNowMSec();
    
    float deltaTime = currTime - lastTime;
    float currCount = MotorEncoder.Counts();
    currPosition = currCount;
    float deltaCounts = currCount - lastEncoderCount;

    // //wait until count has changed enough by at least epsilon, or it has not changed enough in 1ms and remeasure
    if((fabs(deltaCounts) < velocityEpsilon) &&  (deltaTime >= velocityLoopTimerMsMin) && (deltaTime <= velocityLoopTimerMsMax)){
        return currentVelocity;
    }

    if (motorDirection == Direction::BACKWARD) {
        deltaCounts = -deltaCounts;
    }

        
    if(deltaTime < velocityLoopTimerMsMin){
        // not enough time has passed for a reliable velocity calculation
        return currentVelocity;
    }


    //prevent float error issues
    if (deltaCounts == 0) {
        if (deltaTime >= velocityLoopTimerMsMax) {
            //exceeded max loop time, most likely velocity 0
            currentVelocity = 0;
            return 0;
        } else {
            //return old velocity, wait until next loop to get accurate velocity
            return currentVelocity;
        }
    }

    
    velLoopTime = deltaTime;//update loop time for use in PID loop
    velCurrTime = currTime;

    float rotations = deltaCounts / encoderCountsPerRev;
        
    float distance = rotations * wheelCircumference;

    float deltaTimeSec = deltaTime / 1000.0;

    float velocity = distance / deltaTimeSec;


    //calculate totalDisplacement
    totalDisplacement += distance;

    //telemetry
    telemetryTime[telemetryIndex] = velCurrTime;
    telemetryTargetVel[telemetryIndex] = targetVelocity;
    telemetryEncoder[telemetryIndex] = currPosition;
    telemetryIndex++;
    if(telemetryIndex >= telemetryArrLen){
        //loop through array again once its full
        telemetryIndex = 0;
    }


    //update values for next loop
    lastEncoderCount = currCount;
    lastTime = currTime;

    currentVelocity = velocity;

    return velocity;
}

void Motor::runAtVelocity(float v){
    if(motorMode == Mode::VELOCITY){
        targetVelocity = v;
        // (v > 0) ? motorDirection = Direction::FORWARD : motorDirection = Direction::BACKWARD;
        // if(v ==0){motorDirection = Direction::Idle;}

        float currentVelocity = getVelocity();
        float currTime = TimeNowMSec();
        // float loopTime = (currTime - lastTime);

        if(velLoopTime <= 0){
            velLoopTime = 0.001; //assume smol time if no time passed
        }
        //telemetry current velocity
        telemetryVel[telemetryIndex] = currentVelocity;
        //get PID calculation
        float pidOutput = velocityPID.pidCalcLoopTime(targetVelocity,currentVelocity,velLoopTime);
        pidOut = pidOutput;
        //convert pid output to motor percentage
        float motorPower = (pidOutput/maxSpeed) * 100.0;
        
        motorPower = clamp(motorPower, -100,100);//clamp percentage between -100% and 100%

        //debug stuff
        
        // telemetryPIDOut[telemetryIndex] = pidOutput;
        

        SetPercent(motorPower);
    }else{
        assertError("Assertion Error: Motor is not in VELOCITY mode");
    }
    
}


void Motor::setPID(float P, float I, float D){
    //set constants for PID loop for this specific motor
    velocityPID.setPID(P,I,D);
}

float* Motor::getTelemetryVel(){
    return telemetryVel;
}
float* Motor::getTelemetryPIDOut(){
    return telemetryPIDOut;
}

float* Motor::getTelemetryTime(){
    return telemetryTime;
}
float* Motor::getTelemetryTargetVel(){
    return telemetryTargetVel;
}
int Motor::getTelemetryIndex(){
    return telemetryIndex;
}
int* Motor::getTelemetryEncoder(){
    return telemetryEncoder;
}
unsigned int Motor::getTelemetryLen(){
    return telemetryArrLen;
}








  