#include "Slider.h"
#include <FEHLCD.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

using namespace std;

Slider::Slider() {}
// draws centered slider
// Author: Nathan
Slider::Slider(int y, float width, float val, unsigned int color, unsigned int backCol, float min, float max) {
    barX = (screenWidth / 2.0) - (width / 2.0);
    barY = y;
    // init sliderX to init to middle of bar
    sliderX = barX + (width / 2.0);
    sliderY = barY + (sliderHeight / 2.0);

    barWidth = width;

    sliderValue = val;

    sliderColor = color;
    barColor = backCol;

    sliderMax = max;
    sliderMin = min;
    sliderValue = min + (max - min) / 2.0;
    sliderName = "";
    toggled = false;
}

// draws centered slider with name next to it
// Author: Nathan
Slider::Slider(std::string name, int y, float width, float val, unsigned int color, unsigned int backCol, float min,
               float max, bool _canBeInf) {
    barX = (screenWidth / 2.0) - (width / 2.0);
    barY = y;
    // init sliderX to init to middle of bar
    sliderX = barX + (width / 2.0);
    sliderY = barY + (sliderHeight / 2.0);

    barWidth = width;

    sliderValue = val;

    sliderColor = color;
    barColor = backCol;

    sliderMax = max;
    sliderMin = min;
    canBeInf = _canBeInf;
    sliderValue = min + (max - min) / 2.0;
    sliderName = name;
    toggled = false;
}

// updates and renders slider
// Author: Nathan
void Slider::update() {
    bool withinEllipse = false;
    bool withinBar = false;
    setString(sliderValue);

    if (!LCD.Touch(&touchedX, &touchedY, false)) {
        // wait until touch happens
    } else if (LCD.Touch(&xTrash, &yTrash, false)) {

        // wait until touch releases
        // no joke this is the actual code from FEH documentation -_-
        // if touch is within button boundery then set button state to true
        withinEllipse = checkInEllipse(touchedX, touchedY);
        withinBar =
            touchedX >= barX && touchedX <= (barX + barWidth) && touchedY >= barY && touchedY <= barY + barHeight;
    }
    if (withinEllipse) {

        if (currState == sliderState::inactive) {
            currState = sliderState::active;
        } else if (currState == sliderState::active) {
            currState = sliderState::held;
            toggled = true;
        }

    } else {

        if (currState == sliderState::held) {
            currState = sliderState::released;
        } else if (currState == sliderState::released) {
            currState = sliderState::inactive;
        }
    }

    // if touched on the bar the slider will jump to that location
    if (withinBar) {
        sliderX = touchedX;
    }
    // continue tracking pointer even if its no longer directly on the slider until user lets go
    touched = LCD.Touch(&xTrash, &yTrash, false);

    if (toggled && touched) {

        // move slider to pointer x

        sliderX = touchedX;

        // clamp slider between
        if (sliderX >= barX + barWidth) {
            sliderX = barX + barWidth;
        }
        if (sliderX <= barX) {
            sliderX = barX;
        }
    }
    if (!touched) {
        toggled = false;
    }

    float barPercent = (sliderX - barX) / (barWidth);
    sliderValue = ((sliderMax - sliderMin) * barPercent) + sliderMin;
    if (barPercent == 1.0 && canBeInf) {
        sliderValue = INFINITY;
    }
    // std::cout << barPercent << std::endl;

    draw();
}

// draws slider and bar
// Author: Nathan
void Slider::draw() {
    // slider bar
    LCD.SetFontColor(barColor);
    LCD.DrawRectangle(barX, barY, barWidth, barHeight);

    LCD.SetFontColor(sliderColor);
    // slider oval
    LCD.DrawFilledEllipse(sliderX, sliderY, sliderWidth, sliderHeight);
    // slidervalue text
    LCD.WriteAt(sliderValString, barX + barWidth + 8, sliderY - (sliderHeight / 2.0) - 2);
    LCD.WriteAt(sliderName, barX - (sliderName.length() * LCD.getCharWidth()) - 8, sliderY - (sliderHeight / 2.0) - 2);
}

void Slider::setString(float f) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << f;
    sliderValString = stream.str();
}

void Slider::setValue(float f) {
    sliderValue = f;
    if (f == INFINITY) {
        sliderX = barWidth + barX;
    } else {
        sliderX = ((sliderValue - sliderMin) / (sliderMax - sliderMin)) * barWidth + barX;
    }
}
float Slider::getValue() { return sliderValue; }

// check if the touch is in ellipse
// Author: Nathan
bool Slider::checkInEllipse(int x, int y) {
    // taken from the equation of an ellipse
    // if it is greater than 1 then its not in the ellipse
    // recenters ellipse;
    double dx = x - sliderX;
    double dy = y - sliderY;
    // Check if inside ellipse
    double p = (dx * dx / (sliderWidth * sliderWidth)) + (dy * dy / (sliderHeight * sliderHeight));
    return p <= 1.0;
}

// Returns true on first click
// Author: Nathan
bool Slider::onSliderClicked() {
    bool clicked = false;
    if (currState == sliderState::active) {
        currState = sliderState::held;
        clicked = true;
    }
    return clicked;
}

// Returns true on first release
// Author: Nathan
bool Slider::onSliderReleased() {
    bool released = false;
    if (currState == sliderState::released) {
        currState = sliderState::inactive;
        released = true;
    }
    return released;
}

// Returns true while held
// Author: Nathan
bool Slider::getHeld() { return toggled; }
