#include "HolonomicTriangleDrive.h"
#include <limits>
#include <string>
#include <FEHLCD.h>

using namespace std;


float dot(float a[], float b[]){
    return a[0] * b[0] + a[1] * b[1];
}

float magnitude(float v[]){
    return std::sqrt((v[0] * v[0]) + (v[1] * v[1]));
}

//Projection magnitude of vector `a` onto vector `v`
//Gives magnitude of the projected 2D vector
//requires v and a are 2D vectors
float proj(float v[], float a[]){
    float vMag = magnitude(v);
    if(vMag == 0){
        return 0; //no divide by 0 error plz
    }
    return ( dot(a,v) / (vMag) );
}



HolonomicTriangleDrive::HolonomicTriangleDrive(FEHMotor::FEHMotorPort F, FEHMotor::FEHMotorPort BL, FEHMotor::FEHMotorPort BR) 
: Front(F,FrontDefaultEncoder, defaultMotorMaxVolt), 
BackLeft(BL,BackLeftDefaultEncoder, defaultMotorMaxVolt),
BackRight(BR,BackRightDefaultEncoder,defaultMotorMaxVolt)
{
    FrontPort = F;
    BackLeftPort = BL;
    BackRightPort = BR;

    FrontEncoder = FrontDefaultEncoder;
    BackLeftEncoder = BackLeftDefaultEncoder;
    BackRightEncoder = BackLeftDefaultEncoder;

    motorMaxVolt = defaultMotorMaxVolt;

    velocityControl = false;

    //initialize PID constants
    Front.setPID(M1PID[0],M1PID[1],M1PID[2]);
    BackLeft.setPID(M2PID[0],M2PID[1],M2PID[2]);
    BackRight.setPID(M3PID[0],M3PID[1],M3PID[2]);
}

HolonomicTriangleDrive::HolonomicTriangleDrive(FEHMotor::FEHMotorPort F, FEHMotor::FEHMotorPort BL, FEHMotor::FEHMotorPort BR, float maxVolt) 
: Front(F,FrontDefaultEncoder, maxVolt), 
BackLeft(BL,BackLeftDefaultEncoder, maxVolt),
BackRight(BR,BackRightDefaultEncoder,maxVolt)
{
    FrontPort = F;
    BackLeftPort = BL;
    BackRightPort = BR;

    FrontEncoder = FrontDefaultEncoder;
    BackLeftEncoder = BackLeftDefaultEncoder;
    BackRightEncoder = BackLeftDefaultEncoder;

    motorMaxVolt = maxVolt;

    velocityControl = false;

    //initialize PID constants
    Front.setPID(M1PID[0],M1PID[1],M1PID[2]);
    BackLeft.setPID(M2PID[0],M2PID[1],M2PID[2]);
    BackRight.setPID(M3PID[0],M3PID[1],M3PID[2]);
}

HolonomicTriangleDrive::HolonomicTriangleDrive(FEHMotor::FEHMotorPort F,FEHIO::FEHIOPin E1, FEHMotor::FEHMotorPort BL,FEHIO::FEHIOPin E2, FEHMotor::FEHMotorPort BR,FEHIO::FEHIOPin E3, float maxVolt)
: Front(F,E1, maxVolt), 
BackLeft(BL,E2, maxVolt),
BackRight(BR,E3,maxVolt)
{
    FrontPort = F;
    BackLeftPort = BL;
    BackRightPort = BR;

    FrontEncoder = E1;
    BackLeftEncoder = E2;
    BackRightEncoder = E3;

    motorMaxVolt = maxVolt;

    velocityControl = false;

    //initialize PID constants
    Front.setPID(M1PID[0],M1PID[1],M1PID[2]);
    BackLeft.setPID(M2PID[0],M2PID[1],M2PID[2]);
    BackRight.setPID(M3PID[0],M3PID[1],M3PID[2]);
}

//sets target movement vector
void HolonomicTriangleDrive::setMovementVector(float x, float y, float theta){
    MovementVector[0] = x;
    MovementVector[1] = y;
    MovementVector[2] = theta;
}

//Updates all motor states every cycle
void HolonomicTriangleDrive::update(){
    float fSpd = proj(M1, MovementVector);
    float blSpd = proj(M2, MovementVector);
    float brSpd = proj(M3, MovementVector);

    float rotationPower = MovementVector[2];
    float angularVelocity = clamp(rotationPower,-maxRotationSpeed,maxRotationSpeed);

    //apply rotation
    fSpd += angularVelocity;
    blSpd += angularVelocity;
    brSpd += angularVelocity;

    //get max speed out of three motors

    float maxSpeed = std::max(fabs(fSpd), std::max(fabs(blSpd), fabs(brSpd)));


    
    

    //scales speed down so the movement vector is still correct
    if (maxSpeed > maxSpeedPercent) {
        fSpd *= (maxSpeedPercent / maxSpeed);
        blSpd *= (maxSpeedPercent / maxSpeed);
        brSpd *= (maxSpeedPercent / maxSpeed);
    }

    FrontSped = clamp(fSpd,-maxSpeed,maxSpeed);
    BackLeftSped = clamp(blSpd,-maxSpeed,maxSpeed);
    BackRightSped = clamp(brSpd,-maxSpeed,maxSpeed);


    if(velocityControl){

        Front.runAtVelocity(FrontSped * motorMaxVelocity);
        BackLeft.runAtVelocity(BackLeftSped * motorMaxVelocity);
        BackRight.runAtVelocity(BackRightSped * motorMaxVelocity);
    }else{
        
        Front.SetPercent(FrontSped * 100);
        BackLeft.SetPercent(BackLeftSped * 100);
        BackRight.SetPercent(BackRightSped * 100);
    }
    
}

void HolonomicTriangleDrive::stop(){
    setMovementVector(0,0,0);
    Front.Stop();
    BackLeft.Stop();
    BackRight.Stop();
}

float HolonomicTriangleDrive::getFrontSpeed(){return FrontSped;}
float HolonomicTriangleDrive::getBackLeftSpeed(){return BackLeftSped;}
float HolonomicTriangleDrive::getBackRightSpeed(){return BackRightSped;}

float HolonomicTriangleDrive::getFrontPosition(){return Front.getCounts();}
float HolonomicTriangleDrive::getBackLeftPosition(){return BackLeft.getCounts();}
float HolonomicTriangleDrive::getBackRightPosition(){return BackRight.getCounts();}

float HolonomicTriangleDrive::getFrontVelocity(){return Front.getVelocity();}
float HolonomicTriangleDrive::getBackLeftVelocity(){return BackLeft.getVelocity();}
float HolonomicTriangleDrive::getBackRightVelocity(){return BackRight.getVelocity();}

float* HolonomicTriangleDrive::getPose(){
    return Pose;
}


void HolonomicTriangleDrive::setPose(float x, float y, float theta){
    Pose[0] = x;
    Pose[1] = y;
    Pose[2] = theta;
}

void HolonomicTriangleDrive::setTargetPose(float x, float y, float theta){
    TargetPose[0] = x;
    TargetPose[1] = y;
    TargetPose[2] = theta;
}

void HolonomicTriangleDrive::updatePose(){
    Pose[0] = (Front.getTotalDisplacement() * cos(M1[0])) + (BackLeft.getTotalDisplacement() * cos(M2[0])) + (BackRight.getTotalDisplacement() * cos(M3[0]));//x component
    Pose[1] = (Front.getTotalDisplacement() * sin(M1[1])) + (BackLeft.getTotalDisplacement() * sin(M2[1])) + (BackRight.getTotalDisplacement() * sin(M3[1]));//y component
    float robotRadius = 2;
    Pose[2] = (Front.getTotalDisplacement() * sin(M1[0] - M_PI / 2) + BackLeft.getTotalDisplacement() * sin(M2[0] - M_PI / 2) + BackRight.getTotalDisplacement() * sin(M3[0] - M_PI / 2)) / robotRadius;//theta rotation component
}

void HolonomicTriangleDrive::runToPose(){
    float deltaX = TargetPose[0] - Pose[0];
    float deltaY = TargetPose[1] - Pose[1];
    //Target theta
    float targetTheta = TargetPose[2];

    MovementVector[0] = deltaX;
    MovementVector[1] = deltaY;
    MovementVector[2] = targetTheta;
    
    update();  
}

void HolonomicTriangleDrive::turnToTheta(float theta){
    //Get rotation speed
    float currentTheta = Pose[2];
    float targetTheta = TargetPose[2];
    float deltaTheta = targetTheta - currentTheta;
    float angularVelocity = clamp(deltaTheta * rotationGain,-maxRotationSpeed,maxRotationSpeed);
    MovementVector[2] = angularVelocity;
}

void HolonomicTriangleDrive::toggleVelocityControl(bool b){
    velocityControl = b;
    b ? setMotorsToRunAtVelocityMode():setMotorsToRunToPositionMode();
}

void HolonomicTriangleDrive::setMotorsToRunAtVelocityMode(){
    Front.setMode(Motor::Mode::VELOCITY);
    BackLeft.setMode(Motor::Mode::VELOCITY);
    BackRight.setMode(Motor::Mode::VELOCITY);
    }
void HolonomicTriangleDrive::setMotorsToPowerMode(){
    Front.setMode(Motor::Mode::POWER);
    BackLeft.setMode(Motor::Mode::POWER);
    BackRight.setMode(Motor::Mode::POWER);
}
void HolonomicTriangleDrive::setMotorsToRunToPositionMode(){
    Front.setMode(Motor::Mode::RUN_TO_POSITION);
    BackLeft.setMode(Motor::Mode::RUN_TO_POSITION);
    BackRight.setMode(Motor::Mode::RUN_TO_POSITION);
}

void HolonomicTriangleDrive::setMotorPID(int motor, float P, float I, float D){
    
    switch(motor){
        case 1:
        //motor1
        Front.setPID(P,I,D);
        break;
        case 2:
        //motor2
        BackLeft.setPID(P,I,D);
        break;
        case 3:
        //motor3
        BackRight.setPID(P,I,D);
        break;
    }
}

float HolonomicTriangleDrive::getFrontTargetVel(){
    return Front.getTargetVelocity();
}

float HolonomicTriangleDrive::getFrontPIDOut(){
    return Front.getPIDOut();
}