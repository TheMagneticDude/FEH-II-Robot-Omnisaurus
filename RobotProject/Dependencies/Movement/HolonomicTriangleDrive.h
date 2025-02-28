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
#include "Motor.h"



using namespace std;


class HolonomicTriangleDrive {
  //default PID constants
//   #define KP 0.00001
// #define KI 0.0000001
// #define KD 0.00001
  private:
//Front
//        O
//       /   \
//      /     \
// x---(  )---|
//      \     /
//       \   /
//        O
 
  Motor Front;//M1
  const float M1PID[3] = {0.07,0.0000001,0.00001};


//BackLeft
//        O
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        x

  Motor BackLeft;//M2
  const float M2PID[3] = {0.07,0.0000001,0.00001};


//BackRight
//        x
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        0
  Motor BackRight;//M3
  const float M3PID[3] = {0.07,0.0000001,0.00001};



  //motor vector directions
  float M1[2] = {1,0};
  float M2[2] = {-0.5,(std::sqrt(3) / 2.0)};
  float M3[2] = {-0.5,-(std::sqrt(3) / 2.0)};
  float MovementVector[3] = {0,0,0}; //<x,y,theta> components
  // float targetTheta = 0; //0 degrees is default


  float Pose[3] = {0,0,0}; //x,y,theta

  float TargetPose[3] = {0,0,0}; //x,y,theta

    
  public:
  FEHMotor::FEHMotorPort FrontPort;
  FEHIO::FEHIOPin FrontEncoder;
  FEHMotor::FEHMotorPort BackLeftPort;
  FEHIO::FEHIOPin BackLeftEncoder;
  FEHMotor::FEHMotorPort BackRightPort;
  FEHIO::FEHIOPin BackRightEncoder;


  //default encoder vals
  const FEHIO::FEHIOPin FrontDefaultEncoder = FEHIO::P0_0;
  const FEHIO::FEHIOPin BackLeftDefaultEncoder = FEHIO::P0_1;
  const FEHIO::FEHIOPin BackRightDefaultEncoder = FEHIO::P0_2;

  float FrontSped = 0;
  float BackLeftSped = 0;
  float BackRightSped = 0;

  float motorMaxVolt;

  const float defaultMotorMaxVolt = 9.0;

  //max speed percentage
  const float maxSpeedPercent = 1;

  const float motorMaxVelocity = 12;//inch per sec

  //max rotation speed
  const float maxRotationSpeed = 1;
  //how aggresivley robot rotates towards the target angle
  //will need to tune for robot
  const float rotationGain = 0.1;

  bool velocityControl;

  HolonomicTriangleDrive(FEHMotor::FEHMotorPort Front, FEHMotor::FEHMotorPort BackLeft, FEHMotor::FEHMotorPort BackRight);
  HolonomicTriangleDrive(FEHMotor::FEHMotorPort Front, FEHMotor::FEHMotorPort BackLeft, FEHMotor::FEHMotorPort BackRight, float maxVolt);
  HolonomicTriangleDrive(FEHMotor::FEHMotorPort Front,FEHIO::FEHIOPin E1, FEHMotor::FEHMotorPort BackLeft,FEHIO::FEHIOPin E2, FEHMotor::FEHMotorPort BackRight,FEHIO::FEHIOPin E3, float maxVolt);

  void setMovementVector(float x, float y, float theta);
  void update();
  void stop();

  float getFrontSpeed();
  float getBackLeftSpeed();
  float getBackRightSpeed();

  float getFrontPosition();
  float getBackLeftPosition();
  float getBackRightPosition();

  float getFrontVelocity();
  float getBackLeftVelocity();
  float getBackRightVelocity();

  void setPose(float x, float y, float theta);
  void setTargetPose(float x, float y, float theta);
  void turnToTheta(float theta);
  void runToPose();
  void updatePose();
  void toggleVelocityControl(bool b);

  void setMotorsToRunAtVelocityMode();
  void setMotorsToPowerMode();
  void setMotorsToRunToPositionMode();


 
    
};
