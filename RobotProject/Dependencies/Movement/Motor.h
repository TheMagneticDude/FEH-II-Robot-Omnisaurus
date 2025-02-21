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

  DigitalEncoder MotorEncoder;

  FEHIO::FEHIOPin encoderPort;

  float currPosition;
  float targetPos;

  float encoderCountsPerRev;
  
  //VEX Omni wheel is about 2.5 inch in diameter
  const float wheelCircumference = M_PI * 2.5;
  
  
  
  public:
  
  Motor(FEHMotor::FEHMotorPort p, float maxvolt, FEHIO::FEHIOPin encoderPort, float countsperrev);
  Motor(FEHMotor::FEHMotorPort p,FEHIO::FEHIOPin encoderPort, float countsperrev);
  void SetPercent(float percent);
  void Stop();
  void runToPosition(float pos);
  void runToPosition();
  void resetEncoderCounts();
  float getCounts();
  void runAtVelocity(float v);
};
