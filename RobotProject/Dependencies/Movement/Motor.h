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
#include <math.h>
#include <uart.h>
#include "../Dependencies/VelocityPID.h"





using namespace std;

inline float clamp(float x, float min, float max){
  if(x <= min){
      return min;
  }
  if(x >= max){
      return max;
  }
  return x;
}




class Motor {
  private:
  FEHMotor::FEHMotorPort port;
  const float defaultMotorMaxVolt = 9.0;
  float MotorMaxVolt;
  float maxSpeed = 16;
  FEHMotor M;
  

  

  

  DigitalEncoder MotorEncoder;

  FEHIO::FEHIOPin encoderPort;

  float currPosition;
  float targetPos;


  bool velocityLoopTimerPass;

  VelocityPID velocityPID;
  float targetVelocity;
  float currentVelocity;

  //For IGWAN is 318
  float encoderCountsPerRev;
  const unsigned int defaultCountsPerRev = 318;
  
  //VEX Omni wheel is about 2.5 inch in diameter
  const float wheelCircumference = M_PI * 2.5;

  float lastEncoderCount = 0;
  float lastTime = 0;

  float pidOut;
  
  float totalDisplacement;
  
  
  public:

  //enum for motor mode always represented by unsigned int
  enum class Mode : uint8_t {
    RUN_TO_POSITION,
    VELOCITY,
    POWER,
  };

  //motor movement direction when not using power (voltage) control
  enum class Direction : uint8_t {
    FORWARD,
    Idle,
    BACKWARD
  };

  Mode motorMode;

  Direction motorDirection;

  Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin encoderPort, float maxvolt);
  Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin encoderPort, float maxvolt, float countsperrev);
  void setMode(Mode m);
  void SetPercent(float percent);
  void Stop();
  void setTargetPos(float pos);
  void runToPosition();
  void resetEncoderCounts();
  float getCounts();
  float getVelocity();
  float getTargetVelocity();
  float getTotalDisplacement();
  float getPIDOut();
  void runAtVelocity(float v);
  void setPID(float P, float I, float D);
  
};
