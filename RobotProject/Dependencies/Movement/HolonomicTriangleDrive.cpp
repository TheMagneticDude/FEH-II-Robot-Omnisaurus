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

const float DEG_TO_RAD = M_PI / 180.0;
float deg2rad(float degrees) {
    return degrees * DEG_TO_RAD;
}

const float RAD_TO_DEG = 180.0 / M_PI;
float rad2deg(float rad){
    return rad * RAD_TO_DEG;
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
    BackRightEncoder = BackRightDefaultEncoder;

    motorMaxVolt = defaultMotorMaxVolt;

    velocityControl = false;
    reachedTargetPose = false;
    distanceSet = false;


    prevTime = TimeNowMSec();

    Pose[0] = 0;
    Pose[1] = 0;
    Pose[2] = 0;

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
    BackRightEncoder = BackRightDefaultEncoder;

    motorMaxVolt = maxVolt;

    velocityControl = false;
    reachedTargetPose = false;
    distanceSet = false;


    prevTime = TimeNowMSec();

    Pose[0] = 0;
    Pose[1] = 0;
    Pose[2] = 0;

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
    reachedTargetPose = false;
    distanceSet = false;


    prevTime = TimeNowMSec();

    Pose[0] = 0;
    Pose[1] = 0;
    Pose[2] = 0;

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
    //update pose
    updatePose();

    

    float fSpd = proj(M1, MovementVector);
    float blSpd = proj(M2, MovementVector);
    float brSpd = proj(M3, MovementVector);

    float rotationPower = MovementVector[2];
    angularVelocity = clamp(rotationPower,-maxRotationSpeed,maxRotationSpeed);

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

void HolonomicTriangleDrive::driveByDistance(float x, float y, float theta){
    if(!distanceSet){
        TargetPose[0] += x;
        TargetPose[1] += y;
        TargetPose[2] += theta;
        distanceSet = true;
    }
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

float* HolonomicTriangleDrive::getTargetPos(){
    return TargetPose;
}

float* HolonomicTriangleDrive::getMovementVector(){ return MovementVector;}


void HolonomicTriangleDrive::setPose(float x, float y, float theta){
    updatePose();//get curr pose
    // PoseOffset[0] = x - Pose[0];
    // PoseOffset[1] = y - Pose[1];
    Pose[0] = x;
    Pose[1] = y;

    Pose[2] = -theta; //for now bc theta isnt being calculated

    updatePose();//apply offset to pose 
}

void HolonomicTriangleDrive::setTargetPose(float x, float y, float theta){
    isStalled = false;
    prevPose[0] = Pose[0];
    prevPose[1] = Pose[1];
    prevPose[2] = Pose[2];

    TargetPose[0] = x;
    TargetPose[1] = y;
    TargetPose[2] = theta;
}

void HolonomicTriangleDrive::updatePose(){
    float currTime = TimeNowMSec();

    float currFront = Front.getTotalDisplacement();
    float currBL = BackLeft.getTotalDisplacement();
    float currBR = BackRight.getTotalDisplacement();


    //delta counts
    float d1 = currFront - prevEncoderFront;
    float d2 = currBL - prevEncoderBackLeft;
    float d3 = currBR - prevEncoderBackRight;

    prevEncoderFront = currFront;
    prevEncoderBackLeft = currBL;
    prevEncoderBackRight = currBR;


    float dxLocal = (d1 * M1[0] + d2 * M2[0] + d3 * M3[0]) / 3.0;
    float dyLocal = (d1 * M1[1] + d2 * M2[1] + d3 * M3[1]) / 3.0;


    //estimate theta
    float robotRadius = 4;

    float frontVel = Front.getVelocity();
    float backLeftVel = BackLeft.getVelocity();
    float backRightVel = BackRight.getVelocity();


    float deltaTime = (currTime - prevTime) / 1000;//convert to sec

    if (deltaTime <= 0.001) {
        deltaTime = 0.001;
    }

    float inchPerCount = M_PI * 2.5 / 318.0;
    float deltaProjectedAngle = inchPerCount * ( (d1/robotRadius) + (d2 / (sqrt(3) * robotRadius)) - (d3 / (sqrt(3) * robotRadius)) );


    float thetaRad = deg2rad(Pose[2]);
    float dxGlobal = dxLocal * cos(thetaRad) - dyLocal * sin(thetaRad);
    float dyGlobal = dxLocal * sin(thetaRad) + dyLocal * cos(thetaRad);



    //update global pose
    Pose[0] += dxGlobal;
    Pose[1] += dyGlobal;
    // Pose[2] += rad2deg(deltaProjectedAngle);

    //Wraps theta to [0,360]
    // Pose[2] = fmod(Pose[2], 360.0);
    // if (Pose[2] < 0) {
    //     Pose[2] += 360.0;
    // }


    prevTime = TimeNowMSec();
}

void HolonomicTriangleDrive::runToPose(){
    updatePose();
    float deltaX = TargetPose[0] - Pose[0];
    float deltaY = TargetPose[1] - Pose[1];
    float deltaTheta = TargetPose[2] - Pose[2];

    // use while to safeguard against angles outside of -360 to 360 range
    // just in case, it should never get stuck in an infinite loop
    // while (deltaTheta > 180) {
    //     deltaTheta -= 360;
    // }
    // while (deltaTheta < -180) {
    //     deltaTheta += 360;
    // }

    // if(fabs(deltaTheta) < positionEpsilon){deltaTheta  = 0;}s
    if(fabs(deltaX) < positionEpsilon){deltaX  = 0;}
    if(fabs(deltaY) < positionEpsilon){deltaY  = 0;}


    
    float currThetaRad = deg2rad(Pose[2]);
    //current theta with respect to map

    float localX =  deltaX * cos(currThetaRad) + deltaY * sin(currThetaRad);
    float localY = -deltaX * sin(currThetaRad) + deltaY * cos(currThetaRad);

    float magnitude = sqrt(localX * localX + localY * localY);

    if (magnitude > motorMaxVelocity) {
        localX = localX / magnitude * motorMaxVelocity;
        localY = localY / magnitude * motorMaxVelocity;
    }

    MovementVector[0] = clamp(kp_translational*localX,-motorMaxVelocity,motorMaxVelocity);
    MovementVector[1] = clamp(kp_translational*localY,-motorMaxVelocity,motorMaxVelocity);
    // MovementVector[2] = clamp(kp_rotational*deltaTheta,-maxRotationSpeed,maxRotationSpeed);

    

    if(fabs(deltaX) < positionEpsilon && fabs(deltaY) < positionEpsilon){
        MovementVector[0] = 0;
        MovementVector[1] = 0;
        MovementVector[2] = 0;
        stop();
        reachedTargetPose = true;
        distanceSet = false;
    }else{
        reachedTargetPose = false;
        update();
    }
}

void HolonomicTriangleDrive::runToPoseLim(float maxVel){
    updatePose();
    float deltaX = TargetPose[0] - Pose[0];
    float deltaY = TargetPose[1] - Pose[1];
    float deltaTheta = TargetPose[2] - Pose[2];

    //use while to safeguard against angles outside of -360 to 360 range
    //just in case, it should never get stuck in an infinite loop
    //wrap to [-180,180) (will never reach +180)
    while (deltaTheta > 180) {
        deltaTheta -= 360;
    }
    while (deltaTheta < -180) {
        deltaTheta += 360;
    }

    if(fabs(deltaTheta) < positionEpsilon){deltaTheta  = 0;}
    if(fabs(deltaX) < positionEpsilon){deltaX  = 0;}
    if(fabs(deltaY) < positionEpsilon){deltaY  = 0;}


    float currThetaRad = deg2rad(Pose[2]);
    //current theta with respect to map

    float localX =  deltaX * cos(currThetaRad) + deltaY * sin(currThetaRad);
    float localY = -deltaX * sin(currThetaRad) + deltaY * cos(currThetaRad);

    float magnitude = sqrt(localX * localX + localY * localY);

    if (magnitude > maxVel) {
        localX = localX / magnitude * maxVel;
        localY = localY / magnitude * maxVel;
    }

    

    MovementVector[0] = clamp(kp_translational*localX,-maxVel,maxVel);
    MovementVector[1] = clamp(kp_translational*localY,-maxVel,maxVel);
    // MovementVector[2] = clamp(kp_rotational*deltaTheta,-maxRotationSpeed,maxRotationSpeed);

    

    if(fabs(deltaX) < positionEpsilon && fabs(deltaY) < positionEpsilon){
        MovementVector[0] = 0;
        MovementVector[1] = 0;
        MovementVector[2] = 0;
        stop();
        reachedTargetPose = true;
        distanceSet = false;
    }else{
        reachedTargetPose = false;
        update();
    }
}

void HolonomicTriangleDrive::runTilStalled(float maxVel){
    if (isStalled || reachedTargetPose) {
        stop();
        return;
    }

    updatePose();

    runToPoseLim(maxVel);

    //stall detection based on encoder velocity
    float encoderVels[3] = {
        Front.getVelocity(),
        BackLeft.getVelocity(),
        BackRight.getVelocity()
    };

    //check that all are below threashold
    bool allBelow = true;
    bool anyAbove = false;
    for (int i = 0; i < 3; ++i) {
        float vel = fabs(encoderVels[i]);
        if (vel > velocityThreshold) {
            anyAbove = true;
            allBelow = false;
            break;
        }
    }

    float currTime = TimeNowMSec();
    if (runStallTime == 0) {
        runStallTime = currTime;
    }

    bool debouncePassed = (currTime - lastAboveThresholdTime >= stallDebounceDuration);
    // bool maxTimeExceeded = (currTime - runStallTime >= maxStallTime);

    if (anyAbove) {
        hasMoved = true;
        lastAboveThresholdTime = currTime;
    }

    if (!allBelow) {
        lastAboveThresholdTime = currTime; // reset debounce timer
    }

    if (allBelow && debouncePassed && (hasMoved)) {
        isStalled = true;
    } else {
        isStalled = false;
    }

    if(getReachedTargetPos()){
        isStalled = true;//just in case
    }
}

bool HolonomicTriangleDrive::isCurrStalled(){
    return isStalled;
}

void HolonomicTriangleDrive::turnToTheta(float theta){
    //Get rotation speed
    float currentTheta = Pose[2];
    float targetTheta = TargetPose[2];
    float deltaTheta = (targetTheta - currentTheta) / 360.0;
    float angularVelocity = clamp(deltaTheta * rotationGain,-maxRotationSpeed,maxRotationSpeed);
    MovementVector[2] = angularVelocity;
}

void HolonomicTriangleDrive::toggleVelocityControl(bool b){
    velocityControl = b;
    b ? setMotorsToRunAtVelocityMode():setMotorsToPowerMode();
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

Motor* HolonomicTriangleDrive::getMotorInstance(unsigned int i){
    switch(i){
        case 1:
        return &Front;
        case 2:
        return &BackLeft;
        case 3:
        return &BackRight;
    }
}

void HolonomicTriangleDrive::updateMotorDistances(){
        Front.getVelocity();
        BackLeft.getVelocity();
        BackRight.getVelocity();
}

void HolonomicTriangleDrive::resetMotorCounts(){
    Front.resetEncoderCounts();
    BackLeft.resetEncoderCounts();
    BackRight.resetEncoderCounts();
}

float HolonomicTriangleDrive::getFrontTargetVel(){
    return Front.getTargetVelocity();
}
float HolonomicTriangleDrive::getBackLeftTargetVel(){
    return BackLeft.getTargetVelocity();
}
float HolonomicTriangleDrive::getBackRightTargetVel(){
    return BackRight.getTargetVelocity();
}

float HolonomicTriangleDrive::getFrontPIDOut(){
    return Front.getPIDOut();
}
float HolonomicTriangleDrive::getBackLeftPIDOut(){
    return BackLeft.getPIDOut();
}
float HolonomicTriangleDrive::getBackRightPIDOut(){
    return BackRight.getPIDOut();
}

bool HolonomicTriangleDrive::getReachedTargetPos(){
    return reachedTargetPose;
}