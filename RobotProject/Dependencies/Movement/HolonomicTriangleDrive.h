#pragma once

#include <FEHLCD.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>
#include <cmath>



using namespace std;


class HolonomicTriangleDrive {
  private:
//Front
//        O
//       /   \
//      /     \
// x---(  )---|
//      \     /
//       \   /
//        O
 
    FEHMotor Front;//M1


//BackLeft
//        O
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        x

    FEHMotor BackLeft;//M2



//BackRight
//        x
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        0
    FEHMotor BackRight;//M3



  //motor vector directions
  float M1[2] = {1,0};
  float M2[2] = {-(std::sqrt(3) / 2.0),-0.5};
  float M3[2] = {(std::sqrt(3) / 2.0),-0.5};
  float MovementVector[2] = {0,0}; //<x,y> components
  float targetTheta = 0; //0 degrees is default


  float Pose[3] = {0,0,0}; //x,y,theta

  float TargetPose[3] = {0,0,0}; //x,y,theta

    
  public:
  FEHMotor::FEHMotorPort FrontPort;
  FEHMotor::FEHMotorPort BackLeftPort;
  FEHMotor::FEHMotorPort BackRightPort;

  float FrontSped = 0;
  float BackLeftSped = 0;
  float BackRightSped = 0;

  const float defaultMotorMaxVolt = 9.0;

  //max speed percentage
  const float maxSpeedPercent = 1;

  //max rotation speed
  const float maxRotationSpeed = 1;
  //how aggresivley robot rotates towards the target angle
  //will need to tune for robot
  const float rotationGain = 0.1;

  HolonomicTriangleDrive::HolonomicTriangleDrive(FEHMotor::FEHMotorPort Front, FEHMotor::FEHMotorPort BackLeft, FEHMotor::FEHMotorPort BackRight);
  void setMovementVector(float x, float y);
  void update();
  void setPose(float x, float y, float theta);
  void setTargetPose(float x, float y, float theta);
  void calculatePose();


  
    
};
