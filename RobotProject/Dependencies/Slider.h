#pragma once
#include "Grid.h"
#include <FEHLCD.h>
#include <chrono>
#include <string>

using namespace std;

class Slider {
  private:
    const unsigned int screenWidth = 320;
    const unsigned int screenHeight = 240;

    const unsigned int defaultNormColor = WHITE;
    const unsigned int defaultTriggeredColor = GRAY;

    unsigned int sliderColor;
    unsigned int barColor;

    float sliderValue;
    std::string sliderValString;
    std::string sliderName;
    float sliderMax;
    float sliderMin;
    bool canBeInf;

    float sliderX;
    float sliderY;

    float sliderWidth = 6;
    float sliderHeight = 4;

    float barX;
    float barY;

    float touchedX;
    float touchedY;
    float xTrash;
    float yTrash;

    float barWidth;
    float barHeight = 5;

    bool toggled;
    bool touched = false;

    enum class sliderState {
        inactive,
        active,  // first clicked
        held,    // held, being dragged
        released // first released
    };
    sliderState currState = sliderState::inactive;

  public:
    Slider();
    // creates centered Switch
    Slider(int y, float width, float val, unsigned int color, unsigned int backCol, float max, float min);
    Slider(std::string name, int y, float width, float val, unsigned int color, unsigned int backCol, float max,
           float min, bool canBeInf = false);
    void update();
    void draw();
    void setString(float f);
    void setValue(float f);
    float getValue();
    bool checkInEllipse(int x, int y);
    bool onSliderClicked();
    bool onSliderReleased();
    bool getHeld();
};
