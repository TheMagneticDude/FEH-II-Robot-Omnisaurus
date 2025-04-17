#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"
#include "../Dependencies/OptosensorArray.h"


using namespace std;


class LeversDown : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  int &lever;
  FEHServo &arm;

  OptoSensorArray &OptoArr;

  float startTime;
  int i;//path counter 
  bool nextPathTriggered;

  bool didInit;
  bool CDSisRed;

  bool end;
  const std::string commandName = "LeversDown";

  public:
  LeversDown(HolonomicTriangleDrive &dt, int &l, OptoSensorArray opsArr, FEHServo &a);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
