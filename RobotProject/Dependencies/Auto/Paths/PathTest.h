#pragma once

#include <FEHLCD.h>
#include <string>
#include "Command.h"


using namespace std;


class PathTest : Command {
  private:
  
  public:
  PathTest();
  void run() override;    
  bool ended() override;  
  void stop() override;
};
