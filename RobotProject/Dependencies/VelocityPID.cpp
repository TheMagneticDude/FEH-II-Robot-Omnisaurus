#include <limits>
#include <string>
#include "../Dependencies/Movement/Motor.h"
#include "VelocityPID.h"

using namespace std;

//PID Code structure taken from:https://github.com/dcesiel/Velocity-PID-Example/blob/master/pid.h


#define CONTROL_LOOP_TIME 0.001 //1 Millisecond loop time
#define KP 0.00001
#define KI 0.0000001
#define KD 0.00001

VelocityPID::VelocityPID(double desiredRPM, double currentRPM){
//CONSTRUCTOR
//EFFECTS: Initializes PID instance
    error = desiredRPM - currentRPM;
    previousError = error;
    errorSum = 0;
    output = 0;
    //default values
    k_P = KP;
    k_I = KI;
    k_D = KD;
}

int VelocityPID::pidCalc(double desiredRPM, double currentRPM){
//EFFECTS: Returns motor speed based on PID calculation using current
//         motor RPM (I'm assuming that this function will be executed
//         during each control loop
    error = desiredRPM - currentRPM;
    errorSum = errorSum + (error*CONTROL_LOOP_TIME);

    //integral windup prevention
    const double maxIntegral = 1000;
    errorSum = clamp(errorSum, -maxIntegral, maxIntegral);

    errorRateOfChange = (error - previousError)/CONTROL_LOOP_TIME;
    previousError = error;
    //Note: I'm adding the output of the PID to itself because with velocity pid
    //      as error goes to 0 P becomes very small. The velocity will decrease
    //      when you want it to stay at a constant high speed. This works because
    //      your taking the derivative of both sides of the equation
    output += ((k_P*error)+(k_I*errorSum)+(k_D*errorRateOfChange));

    return output;
}

void VelocityPID::setPID(float P, float I, float D){
    k_P = P;
    k_I = I;
    k_D = D;

}