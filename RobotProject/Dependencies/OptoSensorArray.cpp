#include <limits>
#include <string>
#include "OptoSensorArray.h"
#include "../Dependencies/Assert.h"
#include <FEHLCD.h>

using namespace std;

bool isWithin(float x, float min, float max){
    return (x >= min) && (x <= max);
}

bool threshold(float x, float e){
    return (x > e);
}

 OptoSensorArray::OptoSensorArray(FEHIO::FEHIOPin L, FEHIO::FEHIOPin M, FEHIO::FEHIOPin R) : LOpto(L), MOpto(M), ROpto(R){
     LStat = OptoState::INACTIVE;
     MStat = OptoState::INACTIVE;
     RStat = OptoState::INACTIVE;

     arrayState = LineStates::MIDDLE;
     previousCrossed = false;

     suggestedDirection = Direction::LOST;

     valL = 0;
     valM = 0;
     valR = 0;
 }

 void OptoSensorArray::update(){

    valL = LOpto.Value() + Loffset;
    valM = MOpto.Value() + Moffset;
    valR = ROpto.Value() + Roffset;

    LStat = isWithin(valL, triggerThresholdMin, triggerThresholdMax) ? OptoState::ACTIVE : OptoState::INACTIVE;
    MStat = isWithin(valM, triggerThresholdMin, triggerThresholdMax) ? OptoState::ACTIVE : OptoState::INACTIVE;
    RStat = isWithin(valR, triggerThresholdMin, triggerThresholdMax) ? OptoState::ACTIVE : OptoState::INACTIVE;

    switch(arrayState){
        case LineStates::MIDDLE:
            //If left is on the line
            if(LStat == OptoState::ACTIVE){
                arrayState = LineStates::LEFT;
            }
            //If right is on the line
            if(RStat == OptoState::ACTIVE){
                arrayState = LineStates::RIGHT;
            }
            break;

        case LineStates::RIGHT:
            if(RStat == OptoState::INACTIVE){
                arrayState = LineStates::MIDDLE;
            }
            break;

        case LineStates::LEFT:
            if(LStat == OptoState::INACTIVE){
                arrayState = LineStates::MIDDLE;
            }
            break;

        default: // Assert Error. Something is very wrong.
                assertError("Optosensor Array State is invalid.");
                break; 
    }
    //suggest direction
    if (MStat == OptoState::ACTIVE)
        suggestedDirection = Direction::FORWARD;
    else if (LStat == OptoState::ACTIVE)
        suggestedDirection =  Direction::TURN_LEFT;
    else if (RStat == OptoState::ACTIVE)
        suggestedDirection =  Direction::TURN_RIGHT;
    else
        suggestedDirection =  Direction::LOST;
 }

 float OptoSensorArray::optoLValue(){return valL;}
 float OptoSensorArray::optoMValue(){return valM;}
 float OptoSensorArray::optoRValue(){return valR;}

 OptoSensorArray::LineStates OptoSensorArray::getArrayState(){return arrayState;}

 bool OptoSensorArray::crossedLine(){
    bool currentCrossed = isWithin(valM, triggerThresholdMin,triggerThresholdMax);
    bool crossed = (currentCrossed != previousCrossed);
    previousCrossed = currentCrossed;
    return crossed; 
}

 bool OptoSensorArray::isOnLine(){
    //true of left and right are inactive and middle is active
    return (LStat == OptoState::INACTIVE && RStat == OptoState::INACTIVE && MStat == OptoState::ACTIVE);
 }

 OptoSensorArray::Direction OptoSensorArray::getSuggestedDirection(){
    return suggestedDirection;
 }


 //gridline counting

 void OptoSensorArray::startGridLineCounter(int lines){
    gridLineCount = 0;
    lastLineDetected = MStat == OptoState::ACTIVE;
    targetGridLines = lines;
    gridCounterActive = true;
 }

 void OptoSensorArray::updateGridLineCounter() {
    

    if(gridCounterActive){
        unsigned int now = TimeNowMSec();
        bool mTriggered = MStat == OptoState::ACTIVE;

        if((mTriggered != lastLineDetected) && (now - lastLineTriggerTime > debounceTimeMS)){
            //runs only if debounce time is up
            lastLineTriggerTime = now;
            lastLineDetected = mTriggered;
            if(mTriggered){
                gridLineCount++;
            }

            //may need debounc idk
        }
        //stop method once its done
        if(gridLineCount >= targetGridLines){
            gridCounterActive = false;
        }
    }
}

//interrupts grid line counter so it can be called again or whatever
void OptoSensorArray::interruptGridLineCounter(){
    gridCounterActive = false;
    gridLineCount = 0;
}

bool OptoSensorArray::hasReachedGridLineTarget(){
    return !gridCounterActive && (gridLineCount >= targetGridLines);
}

int OptoSensorArray::getGridLineCount(){return gridLineCount;}