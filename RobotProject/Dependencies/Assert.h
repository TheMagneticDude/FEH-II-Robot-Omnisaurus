#pragma once

#include <FEHLCD.h>
#include <string>
#include <cstring>


using namespace std;

//If a method is being used wrong, stick code into an infinite loop with an assertion error
inline void assertError(std::string error){
  LCD.Clear();
  while(true){
    LCD.SetBackgroundColor(LCD.Blue);
    LCD.SetFontColor(LCD.Red);
    LCD.WriteLine(error.c_str());
  }
}

class Assert {
  private:
  public:
};
