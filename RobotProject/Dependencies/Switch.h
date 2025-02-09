#pragma once
#include "Grid.h"
#include <FEHLCD.h>
#include <chrono>
#include <string>

using namespace std;

class Switch {
  private:
    const unsigned int screenWidth = 320;
    const unsigned int screenHeight = 240;

    const unsigned int defaultNormColor = WHITE;
    const unsigned int defaultTriggeredColor = GRAY;

    unsigned int switchColor;
    unsigned int backColor;

    float switchX;
    float switchY;

    float touchedX;
    float touchedY;
    float xTrash;
    float yTrash;

    float switchCenterX;
    float switchCenterY;

    float switchWidth;
    float switchHeight;

    string switchText;

    bool toggled;

    enum class switchState { inactive, active, held, released };
    switchState currState = switchState::inactive;

    // debounce
    float debounceTimer = 50; // millisec
    std::chrono::steady_clock::time_point debounceStart;

  public:
    Switch();
    // creates centered Switch
    Switch(int y, string text, unsigned int color, unsigned int trigColor);
    Switch(int y, string text, unsigned int color, unsigned int bcol, int a, int b);
    bool checkInEllipse(int x, int y);
    bool onSwitchClicked();
    bool onSwitchReleased();
    bool getToggled();
    void setString(std::string s);
    void update();
    void draw();
};
