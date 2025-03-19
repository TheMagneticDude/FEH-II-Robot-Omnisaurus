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
    lastDesiredRPM = 0;
}

float VelocityPID::pidCalc(double desiredRPM, double currentRPM){
    // if(fabs(lastDesiredRPM - desiredRPM) >= 0.1){
    //     //if the last desired RPM is different enough, reset PID variables so no errors occur
    //     error = desiredRPM - currentRPM;
    //     previousError = error;
    //     errorSum = 0;
    //     output = 0;
    // }
    //EFFECTS: Returns motor speed based on PID calculation using current
    //         motor RPM (I'm assuming that this function will be executed
    //         during each control loop
    error = desiredRPM - currentRPM;
    errorSum = errorSum + (error*CONTROL_LOOP_TIME);
    

    //integral windup prevention
    if ((error > 0 && previousError < 0) || (error < 0 && previousError > 0)) {
        errorSum = 0;  // Reset integral term when error sign flips
    }
    const double maxIntegral = 5;
    errorSum = clamp(errorSum, -maxIntegral, maxIntegral);

    errorRateOfChange = (error - previousError)/CONTROL_LOOP_TIME;
    previousError = error;
    //Note: I'm adding the output of the PID to itself because with velocity pid
    //      as error goes to 0 P becomes very small. The velocity will decrease
    //      when you want it to stay at a constant high speed. This works because
    //      your taking the derivative of both sides of the equation
    output = ((k_P*error) + (k_I*errorSum) + (k_D*errorRateOfChange));
    lastDesiredRPM = desiredRPM;
    return output;
}

float VelocityPID::pidCalcLoopTime(double desiredRPM, double currentRPM, double loopTime){
    // if(fabs(lastDesiredRPM - desiredRPM) >= 0.1){
    //     //if the last desired RPM is different enough, reset PID variables so no errors occur
    //     error = desiredRPM - currentRPM;
    //     previousError = error;
    //     errorSum = 0;
    //     output = 0;
    // }
        //EFFECTS: Returns motor speed based on PID calculation using current
        //         motor RPM (I'm assuming that this function will be executed
        //         during each control loop
        error = desiredRPM - currentRPM;
        errorSum = errorSum + (error*loopTime);
        if (loopTime < 0.001) {  // Prevent division issues
            loopTime = 0.001;
        }


        //integral windup prevention
        if ((error > 0 && previousError < 0) || (error < 0 && previousError > 0) || fabs(error) < 0.1) {
            errorSum = 0;
        }

        if(fabs(lastDesiredRPM - desiredRPM) >= 0.1){
            errorSum = 0;
        }
        
        if(fabs(lastDesiredRPM - desiredRPM) >= 0.5){
            //reset output if desired RPM changed enough
            output = 0;
        }
        
        const double maxIntegral = 50;
        errorSum = clamp(errorSum, -maxIntegral, maxIntegral);
    
        errorRateOfChange = (error - previousError)/loopTime;
        previousError = error;
        //Note: I'm adding the output of the PID to itself because with velocity pid
        //      as error goes to 0 P becomes very small. The velocity will decrease
        //      when you want it to stay at a constant high speed. This works because
        //      your taking the derivative of both sides of the equation
        output += ((k_P*error) + (k_I*errorSum) + (k_D*errorRateOfChange));
        lastDesiredRPM = desiredRPM;

        const float MIN_OUTPUT_THRESHOLD = 0.1;
        if (fabs(output) < MIN_OUTPUT_THRESHOLD && fabs(error) > 0.1) {
            output = (output > 0) ? MIN_OUTPUT_THRESHOLD : -MIN_OUTPUT_THRESHOLD;
        }

        return output;
    }

void VelocityPID::setPID(float P, float I, float D){
    k_P = P;
    k_I = I;
    k_D = D;

}