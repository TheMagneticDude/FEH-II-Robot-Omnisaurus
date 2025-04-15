#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class Buttons : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  AnalogInputPin &CDS;

  float startTime;
  int i;//path counter 
  bool nextPathTriggered;

  bool didInit;
  bool CDSisRed;

  bool end;
  const std::string commandName = "Buttons";

  public:
  Buttons(HolonomicTriangleDrive &dt, AnalogInputPin cds);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
