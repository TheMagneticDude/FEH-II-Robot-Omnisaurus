#pragma once

#include <FEHLCD.h>
#include <string>
#include <functional>
#include <vector>
#include "SequencialCOmmand.h"


using namespace std;


class Command{
  private:



  public:
  //default constructure
    Command() = default;
    virtual  void run() = 0;
    virtual bool ended() = 0;
    virtual void stop() = 0;

    //prevents memory leak, kills derived classes when it should
    virtual ~Command() = default;
    
};
