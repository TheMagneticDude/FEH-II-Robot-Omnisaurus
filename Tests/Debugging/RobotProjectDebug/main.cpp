#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRCS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <chrono>

#include <pthread.h>
#include <iostream>


int main(void)
{

    float lastTime = 0;
    float currentVelocity = 0;

    while(true){
        float velocityEpsilon = 60; //approx 20 percent of 318 counts per rev //Min amount of encoder delta to update velocity

        float velocityLoopTimerMsMin = 5;//min loop time of 5 ms  
        float velocityLoopTimerMsMax = 50;//max loop time of 50 ms  
        
    
        float currTime = TimeNowMSec();
        
        float deltaTime = 133;
        // float currCount = MotorEncoder.Counts();
        float deltaCounts = 34;
    
        //wait until count has changed enough by at least epsilon, or it has not changed enough in 1ms and remeasure
        if((fabs(deltaCounts) < velocityEpsilon) &&  (deltaTime >= velocityLoopTimerMsMin) && (deltaTime <= velocityLoopTimerMsMax)){
            return currentVelocity;
        }
    
        // if (motorDirection == Direction::BACKWARD) {
            deltaCounts = -deltaCounts;
        // }
    
            
        if(deltaTime <= 0){
            //no divide by zero error
            return currentVelocity;
        }
    
    
        //prevent float error issues
        if (deltaCounts == 0) {
            if (deltaTime >= velocityLoopTimerMsMax) {
                //exceeded max loop time, most likely velocity 0
                currentVelocity = 0;
                return 0;
            } else {
                //return old velocity, wait until next loop to get accurate velocity
                return currentVelocity;
            }
        }
    
        
        // velLoopTime = deltaTime;//update loop time for use in PID loop
        // velCurrTime = currTime;
    
        float rotations = deltaCounts / 318;
            
        float distance = rotations * M_PI * 2.5;
    
        float deltaTimeSec = deltaTime / 1000.0;
    
        float velocity = distance / deltaTimeSec;
    
    
        //calculate totalDisplacement
        // totalDisplacement += distance;
    
    
    
        //update values for next loop
        // lastEncoderCount = currCount;
        lastTime = currTime;
    
        currentVelocity = velocity;

        LCD.WriteAt(velocity,0,0);
    }
	return 0;
}
