#pragma once

#include <FEHLCD.h>
#include <string>
#include <FEHIO.h>



using namespace std;


class QuadratureEncoder {
  private:
  // DigitalEncoder EA;
  // DigitalEncoder EB;

  DigitalInputPin inputA;
  DigitalInputPin inputB;

  int ticks;

  int prevA;
  int prevB;

  public:
  enum class encoderState : uint32{
    FORWARD,
    IDLE,
    BACKWARD
  };

  encoderState currState;

  QuadratureEncoder(FEHIO::FEHIOPin E1, FEHIO::FEHIOPin E2);
  void update();
  void ResetCounts();
  encoderState getCurrentState();
  //capital so its same as FEHIO
  int Counts();

};
