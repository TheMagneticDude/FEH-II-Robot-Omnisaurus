#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class SelectButton : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  AnalogInputPin &CDS;
  float startTime;
  bool didInit;
  bool CDSisRed;

  bool end;
  const std::string commandName = "SelectButton";

  public:
  SelectButton(HolonomicTriangleDrive &dt, AnalogInputPin &CDS);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
