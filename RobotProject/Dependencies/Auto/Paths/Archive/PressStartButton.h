#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class PressStartButton : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  float startTime;

  bool end;
  const std::string commandName = "PressStartButton";

  public:
  PressStartButton(HolonomicTriangleDrive &dt);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
