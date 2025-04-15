#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class AppleBasketDropoff : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;

  float startTime;
  int i;//path counter 
  bool nextPathTriggered;

  bool end;
  const std::string commandName = "AppleBasketDropoff";

  public:
  AppleBasketDropoff(HolonomicTriangleDrive &dt);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
