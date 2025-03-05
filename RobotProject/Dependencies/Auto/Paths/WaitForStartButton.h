#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class WaitForStartButton : public Command {
  private:
  AnalogInputPin &CDS;
  bool CDSTriggered;
  float startTime;

  bool end;
  const std::string commandName = "Wait for start";

  public:
  WaitForStartButton( AnalogInputPin &CDS);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
