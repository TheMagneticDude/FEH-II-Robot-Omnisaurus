#pragma once

#include <FEHLCD.h>
#include <string>
#include <functional>
#include <vector>
#include "Command.h"
#include "HolonomicTriangleDrive.h"




using namespace std;


class SequencialCommand {
  private:

  //vector push back adds to back and pop back removes off the back
  std::vector<Command> commandList;

  public:
    SequencialCommand();
    void addCommand(Command command);
    void runSequencialCommand();
};
