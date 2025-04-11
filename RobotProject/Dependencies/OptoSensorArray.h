#pragma once

#include <FEHLCD.h>
#include <string>
#include <FEHIO.h>



using namespace std;


class OptoSensorArray {
  private:
  //nonreflective is 3.3V
    //reflective is 0
  const float triggerThresholdMin = 2.76;//min threashold to detect nonreflective line
  const float triggerThresholdMax = 3.3;//max threashold for nonreflective line (0 reflection)
  
  //2.751 avg when on black line
                //L offset = -0.212
                //M offset = +0.101
                //R offset = +0.111

  float Loffset = -0.212 + 0.18033333333;
  float Moffset= 0.101-0.04966666667;
  float Roffset = 0.111-0.13066666667;

  AnalogInputPin LOpto;
  AnalogInputPin MOpto;
  AnalogInputPin ROpto;

  float valL;
  float valM;
  float valR;

  int gridLineCount = 0;
  int targetGridLines = 0;
  bool lastLineDetected = false;
  bool gridCounterActive = false;
  

  bool previousCrossed;

  unsigned int lastLineTriggerTime;
  //1ms debounce
  const unsigned int debounceTimeMS = 1;

  
  public:
    enum class OptoState : uint32{
      INACTIVE,
      ACTIVE
    };

    enum class LineStates : uint32{ 
      MIDDLE, 
      RIGHT, 
      LEFT
     };

    enum class Direction : uint32{
      FORWARD,
      TURN_LEFT,
      TURN_RIGHT,
      LOST
    };

    OptoState LStat;
    OptoState MStat;
    OptoState RStat;

    LineStates arrayState;

    Direction suggestedDirection;

    OptoSensorArray(FEHIO::FEHIOPin L, FEHIO::FEHIOPin M, FEHIO::FEHIOPin R);
    void update();
    void startGridLineCounter(int lines);
    void updateGridLineCounter();
    void interruptGridLineCounter();
    float optoLValue();
    float optoMValue();
    float optoRValue();
    bool getLState();
    bool getMState();
    bool getRState();
    int getGridLineCount();
    LineStates getArrayState();
    Direction getSuggestedDirection();
    bool hasReachedGridLineTarget();
    bool crossedLine();
    bool isOnLine();
};
