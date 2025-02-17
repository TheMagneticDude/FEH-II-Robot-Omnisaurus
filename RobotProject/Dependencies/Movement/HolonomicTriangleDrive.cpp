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

float clamp(float x, float min, float max){
    if(x <= min){
        return min;
    }
    if(x >= max){
        return max;
    }
    return x;
}

HolonomicTriangleDrive::HolonomicTriangleDrive(FEHMotor::FEHMotorPort F, FEHMotor::FEHMotorPort BL, FEHMotor::FEHMotorPort BR) 
: Front(F, defaultMotorMaxVolt), 
BackLeft(BL, defaultMotorMaxVolt),
BackRight(BR,defaultMotorMaxVolt)
{
    FrontPort = F;
    BackLeftPort = BL;
    BackRightPort = BR;
}

//sets target movement vector
void HolonomicTriangleDrive::setMovementVector(float x, float y){
    MovementVector[0] = x;
    MovementVector[1] = y;
}

//Updates all motor states every cycle
void HolonomicTriangleDrive::update(){
    float fSpd = proj(M1, MovementVector);
    float blSpd = proj(M2, MovementVector);
    float brSpd = proj(M3, MovementVector);

    //Get rotation speed
    float currentTheta = Pose[2];
    float deltaTheta = targetTheta - currentTheta;
    float angularVelocity = clamp(deltaTheta * rotationGain,-maxRotationSpeed,maxRotationSpeed);

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

    Front.SetPercent(FrontSped * 100);
    BackLeft.SetPercent(BackLeftSped * 100);
    BackRight.SetPercent(BackRightSped * 100);
}



