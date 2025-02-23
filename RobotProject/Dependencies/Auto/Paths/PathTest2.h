#pragma once

#include <FEHLCD.h>
#include <string>
#include "../Dependencies/Auto/Command.h"
#include "../Dependencies/Movement/HolonomicTriangleDrive.h"


using namespace std;


class PathTest2 : public Command {
  private:
  HolonomicTriangleDrive &drivetrain;
  std::chrono::steady_clock::time_point start;

  bool end;
  const std::string commandName = "PathTest";

  public:
  PathTest2(HolonomicTriangleDrive &dt);
  void run() override;    
  bool ended() override;  
  void stop() override;
  std::string getName() override;
};
