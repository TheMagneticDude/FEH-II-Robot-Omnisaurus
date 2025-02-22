#pragma once

#include <FEHLCD.h>
#include <string>
#include "Command.h"
#include "HolonomicTriangleDrive.h"


using namespace std;


class PathTest : Command {
  private:
  HolonomicTriangleDrive drivetrain;
  bool end;
  std::chrono::steady_clock::time_point start;
  public:
  PathTest(HolonomicTriangleDrive &dt);
  void run() override;    
  bool ended() override;  
  void stop() override;
};
