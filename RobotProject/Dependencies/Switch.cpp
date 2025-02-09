#include "Switch.h"
#include <FEHLCD.h>
#include <iostream>
#include <limits>
#include <math.h>
#include <string>

using namespace std;

Switch::Switch() {}
// draws centered switch
Switch::Switch(int y, string text, unsigned int color, unsigned int bcol) {
    switchX = (screenWidth / 2.0) - ((text.length() / 2.0) * LCD.getCharWidth());
    switchY = y;

    switchCenterX = (screenWidth / 2.0);
    switchCenterY = y + ((text.length() / 2.0) * LCD.getCharHeight());

    switchWidth = text.length() * LCD.getCharWidth();
    switchHeight = LCD.getCharHeight();

    switchText = text;

    switchColor = color;
    backColor = bcol;

    toggled = false;
    debounceStart = std::chrono::steady_clock::now();
}

Switch::Switch(int y, string text, unsigned int color, unsigned int bcol, int a, int b) {
    switchX = (screenWidth / 2.0) - ((text.length() / 2.0) * LCD.getCharWidth());
    switchY = y;

    switchCenterX = (screenWidth / 2.0);
    switchCenterY = y + ((text.length() / 2.0) * LCD.getCharHeight());

    switchWidth = a;
    switchHeight = b;

    switchText = text;

    switchColor = color;
    backColor = bcol;

    toggled = false;
    debounceStart = std::chrono::steady_clock::now();
}

void Switch::update() {
    draw();

    bool withinEllipse = false;

    if (!LCD.Touch(&touchedX, &touchedY, false)) {

        // wait until touch happens
    } else if (LCD.Touch(&xTrash, &yTrash, false)) {
        // wait until touch releases
        // no joke this is the actual code from FEH documentation -_-
        // if touch is within button boundery then set button state to true
        withinEllipse = checkInEllipse(touchedX, touchedY);
    }
    if (withinEllipse) {
        auto currTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - debounceStart).count();

        if (elapsedTime > debounceTimer) {
            if (currState == switchState::inactive) {
                currState = switchState::active;
            } else if (currState == switchState::active) {
                currState = switchState::held;
            }
            if (onSwitchClicked()) {
                toggled = !toggled;
            }
            debounceStart = currTime; // Reset debounce timer after toggling
        }
    } else {
        if (currState == switchState::held) {
            currState = switchState::released;
        } else if (currState == switchState::released) {
            currState = switchState::inactive;
        }
    }
}

bool Switch::checkInEllipse(int x, int y) {
    // taken from the equation of an ellipse
    // if it is greater than 1 then its not in the ellipse
    // recenters ellipse;
    double dx = x - switchCenterX;
    double dy = y - switchCenterY;
    // Check if inside ellipse
    double p = (dx * dx / (switchWidth * switchWidth)) + (dy * dy / (switchHeight * switchHeight));
    return p <= 1.0;
}

bool Switch::onSwitchClicked() {
    bool clicked = false;
    if (currState == switchState::active) {
        currState = switchState::held;
        clicked = true;
    }
    return clicked;
}

bool Switch::onSwitchReleased() {
    bool released = false;
    if (currState == switchState::released) {
        currState = switchState::inactive;
        released = true;
    }
    return released;
}

bool Switch::getToggled() { return toggled; }

void Switch::setString(std::string s) { switchText = s; }

void Switch::draw() {
    // draw background ellipse
    LCD.SetFontColor(backColor);
    LCD.DrawFilledEllipse(switchCenterX, switchCenterY, switchWidth, switchHeight);
    // draw switch circle portion
    LCD.SetFontColor(switchColor);
    if (toggled) {
        LCD.FillCircle(switchCenterX + switchWidth, switchCenterY, switchHeight);
    } else {
        LCD.FillCircle(switchCenterX - switchWidth, switchCenterY, switchHeight);
    }

    LCD.WriteAt(switchText, switchX, switchY);
}
