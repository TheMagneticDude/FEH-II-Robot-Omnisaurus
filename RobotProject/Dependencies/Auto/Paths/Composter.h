#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class Composter : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  FEHMotor &composterArm;
  FEHServo &arm;
  float startTime; 
  int i;//path counter
  bool nextPathTriggered;

  bool end;
  const std::string commandName = "Composter";

  public:
  Composter(HolonomicTriangleDrive &dt, FEHServo &a, FEHMotor &composterArm);
  void init() override;
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
