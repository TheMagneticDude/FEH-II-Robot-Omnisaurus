#pragma once

#include <FEHLCD.h>
#include <string>
#include <functional>
#include <chrono>
#include <vector>
#include "../Movement/HolonomicTriangleDrive.h"



using namespace std;
inline bool timeUp(float start, float durationMs) {
  return (TimeNowMSec() - start >= durationMs);
}


//Threads arent supported :[ boo
// inline void runForAsync(std::function<void()> function, std::chrono::milliseconds duration){
  
//   std::thread([function, duration](){
//     auto start = std::chrono::steady_clock::now();
//     while(timeUp(start,duration)){
//       //call function while time isnt up yet
//       function();
//       //function will typically be the run(); function in a path or command
//     }
//   }).detach();//thread detatches automatically
// }



//helper command to make chrono milisecs easier
inline std::chrono::milliseconds chronMiliSec(int t){
  return std::chrono::milliseconds(t);
}


//Abstract class to set up command structure for paths
class Command{
  private:
  const std::string commandName;
  public:
  //default constructure
    Command() = default;
    virtual void init() = 0;
    virtual  void run() = 0;
    virtual bool ended() = 0;
    virtual void stop() = 0;
    virtual std::string getName() = 0;

    //prevents memory leak, kills derived classes when it should
    virtual ~Command() = default;
    
};
