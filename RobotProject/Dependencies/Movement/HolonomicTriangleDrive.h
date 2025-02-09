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



using namespace std;


class HolonomicTriangleDrive {
  private:
//Front
//        O
//       /   \
//      /     \
// x---(  )---|
//      \     /
//       \   /
//        O
 
    FEHMotor Front;


//BackLeft
//        O
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        x

    FEHMotor BackLeft;



//BackRight
//        x
//       /   \
//      /     \
// O---(  )---|
//      \     /
//       \   /
//        0
    FEHMotor BackRight;
  public:
    
};
