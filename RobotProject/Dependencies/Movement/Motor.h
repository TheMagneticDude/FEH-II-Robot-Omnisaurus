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
  float maxSpeed = 1;
  FEHMotor M;

  //enum for motor mode always represented by unsigned int
  enum class Mode : uint8_t {
    RUN_TO_POSITION,
    VELOCITY,
    POWER,
  };

  //motor movement direction when not using power (voltage) control
  enum class Direction : uint8_t {
    FORWARD,
    BACKWARD
  };

  Mode motorMode;

  DigitalEncoder MotorEncoder;

  FEHIO::FEHIOPin encoderPort;

  float currPosition;
  float targetPos;

  //For IGWAN is 318
  float encoderCountsPerRev;
  const unsigned int defaultCountsPerRev = 318;
  
  //VEX Omni wheel is about 2.5 inch in diameter
  const float wheelCircumference = M_PI * 2.5;


  
  
  
  
  public:
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
  void runAtVelocity(float v);

  
};
