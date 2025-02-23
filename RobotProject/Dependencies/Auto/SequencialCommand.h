#pragma once

#include <FEHLCD.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "Command.h"





using namespace std;


class SequencialCommand {
  private:
  unsigned int currCommand;
  //vector push back adds to back and pop back removes off the back
  //stores as a vector bc any class implement command and stil need to be sequencial
  std::vector<std::unique_ptr<Command>> commandList;

  public:
    SequencialCommand();
    void addCommand(std::unique_ptr<Command> command);
    void runSequencialCommand();
};
