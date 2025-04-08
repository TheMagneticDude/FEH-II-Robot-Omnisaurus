#include <limits>
#include <string>
#include "QuadratureEncoder.h"
#include "../Dependencies/Assert.h"
#include <FEHLCD.h>

using namespace std;



QuadratureEncoder::QuadratureEncoder(FEHIO::FEHIOPin E1, FEHIO::FEHIOPin E2) : inputA(E1), inputB(E2) {
    ticks = 0;
    prevA = 0;
    prevB = 0;

    currState = encoderState::IDLE;
}

void QuadratureEncoder::update(){
//possible states
// A B
// 0 0
// 0 1
// 1 1
// 1 0
    int currA = inputA.Value();
    int currB = inputB.Value();
    //based on diagram: https://www.dynapar.com/hs-fs/hubfs/uploadedImages/_Site_Root/technology/encoder_basics/quadrature.jpg?width=525&name=quadrature.jpg
    //lookup table stolen from here:https://daniellethurow.com/blog/2021/8/30/how-to-use-quadrature-rotary-encoders
    const unsigned int lookupTable[16] = {
        0,  // 00 -> 00 (DIDN’T MOVE)
        -1, // 00 -> 01
        1,  // 00 -> 10
        2,  // 00 -> 11 (invalid)

        1,  // 01 -> 00
        0,  // 01 -> 01 (DIDN’T MOVE)
        2,  // 01 -> 10 (invalid)
        -1, // 01 -> 11

        -1, // 10 -> 00
        2,  // 10 -> 01 (invalid)
        0,  // 10 -> 10 (DIDN’T MOVE)
        1,  // 10 -> 11

        2,  // 11 -> 00 (invalid)
        1,  // 11 -> 01
        -1, // 11 -> 10
        0   // 11 -> 11 (DIDN’T MOVE)
    };

    //shift A over by one and add b to end
    int currBitState = (currA << 1) | currB;
    int prevBitState = (prevA << 1) | prevB;

    //combined prevA,currA,prevB,currB format ie: 0110
    //effectivly shift prev over by 2 and add currBitState onto end
    int combinedBitState = (prevBitState << 2) | currBitState;

    int direction = lookupTable[combinedBitState];

    if(direction == 1){
        currState = encoderState::FORWARD;
    }else if(direction == -1){
        currState = encoderState::BACKWARD;
    }else{
        currState = encoderState::IDLE;
    }



    if(currState == encoderState::FORWARD){ticks++;}
    if(currState == encoderState::BACKWARD){ticks--;}

    //update prev values
    prevA = currA;
    prevB = currB;

    // if(prevA != currA){
    //     //only update values if the square wave changes
    //     prevA = currA;
    // }
    // if(prevB != currB){
    //     prevB = currB;
    // }
    
    
}

void QuadratureEncoder::ResetCounts(){ticks = 0;}

QuadratureEncoder::encoderState QuadratureEncoder::getCurrentState(){return currState;}

int QuadratureEncoder::Counts(){update(); return ticks;}