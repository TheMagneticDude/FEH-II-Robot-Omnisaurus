#include <limits>
#include <string>
#include <FEHLCD.h>
#include "Motor.h"

using namespace std;


Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt) : M(p,maxvolt), MotorEncoder(ep){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = defaultCountsPerRev;
    encoderPort = encoderPort;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;

    getMotorData(p);
}

Motor::Motor(FEHMotor::FEHMotorPort p, FEHIO::FEHIOPin ep,float maxvolt, float countsperrev) : M(p,maxvolt), MotorEncoder(ep){
    port = p;
    MotorMaxVolt = maxvolt;
    encoderCountsPerRev = countsperrev;
    MotorEncoder.ResetCounts();

    //default mode is power
    motorMode = Mode::POWER;

    getMotorData(p);
}

void Motor::setMode(Mode m){
    motorMode = m;
}

void Motor::SetPercent(float percent){
    if(motorMode == Mode::POWER){
        getMotorData(port);
        M.SetPercent(percent);
    }else{
        LCD.WriteLine("Motor is not in POWER mode");
    }
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

        M.SetPercent(power);
    }else{
        LCD.WriteLine("Motor is not in RUN_TO_POSITION mode");
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
float Motor::getVelocity(){
    return 0;
}
void Motor::runAtVelocity(float v){
    if(motorMode == Mode::VELOCITY){
        
    }else{
        LCD.WriteLine("Motor is not in VELOCITY mode");
    }
    
}





//IDK if this will work
//stolen fron FEHPropeller
//basically trying to get the voltage therefore direction and rate of motor
//idk if I can request data over serial connection tho
void Motor::getMotorData(FEHMotor::FEHMotorPort port){
    //make port an unsigned 8 bit integer
    uint8 motor = (unsigned char) port;
    uart_putchar(UART5_BASE_PTR, 0x7F); // Start
      uart_putchar(UART5_BASE_PTR, 0x09); //Request motor data
      uart_putchar(UART5_BASE_PTR, motor); // motor port
      uart_putchar(UART5_BASE_PTR, 0xFF); // End
  
    uint8 speed = uart_getchar(UART5_BASE_PTR);
    uint8 rate = uart_getchar(UART5_BASE_PTR);
  }


  uint8 Motor::getSerialRate(){
    return rate;
  }

  uint8 Motor::getSerialSpeed(){
    return speed;
  }
