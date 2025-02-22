#pragma once

#include <FEHLCD.h>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <vector>
#include "HolonomicTriangleDrive.h"


using namespace std;
inline bool timeUp(std::chrono::steady_clock::time_point start, std::chrono::milliseconds duration) {
  return (std::chrono::steady_clock::now() - start < duration);
}


inline void runForAsync(std::function<void()> function, std::chrono::milliseconds duration){
  
  std::thread([function, duration](){
    auto start = std::chrono::steady_clock::now();
    while(timeUp(start,duration)){
      //call function while time isnt up yet
      function();
      //function will typically be the run(); function in a path or command
    }
  }).detach();//thread detatches automatically
}

//helper command to make chrono milisecs easier
inline std::chrono::milliseconds chronMiliSec(int t){
  return std::chrono::milliseconds(t);
}



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
