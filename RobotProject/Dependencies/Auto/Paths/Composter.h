#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class Composter : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  FEHServo &arm;
  float startTime;
  int i;//path counter
  bool nextPathTriggered;

  bool end;
  const std::string commandName = "AppleBasket";

  public:
  Composter(HolonomicTriangleDrive &dt, FEHServo &a);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
