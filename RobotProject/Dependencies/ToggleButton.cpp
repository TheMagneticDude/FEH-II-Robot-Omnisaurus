#include <FEHLCD.h>
#include <limits>
#include <string>

#include "Sound.h"
#include "ToggleButton.h"

using namespace std;

ToggleButton::ToggleButton() {}
// draws centered button
// Author: Nathan
ToggleButton::ToggleButton(int y, string text, unsigned int color, unsigned int trigColor) {

    buttonX = (screenWidth / 2.0) - ((text.length() / 2.0) * LCD.getCharWidth());
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;

    defaultColor = color;
    triggeredColor = trigColor;
    disabledColor = defaultDisabledColor;
    highlighted = false;
    wasHighlighted = false;

    removed = false;

    currColor = defaultColor;
    // initializes time
    lastPress = std::chrono::high_resolution_clock::now();
}
ToggleButton::ToggleButton(float x, float y, string text) {

    buttonX = x;
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;

    defaultColor = defaultNormColor;
    triggeredColor = defaultTriggeredColor;
    disabledColor = defaultDisabledColor;
    highlighted = false;
    wasHighlighted = false;

    removed = false;

    currColor = defaultColor;
    // initializes time
    lastPress = std::chrono::high_resolution_clock::now();
}

ToggleButton::ToggleButton(float x, float y, string text, unsigned int color, unsigned int trigColor) {
    buttonX = x;
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;

    defaultColor = color;
    triggeredColor = trigColor;
    disabledColor = defaultDisabledColor;
    highlighted = false;
    wasHighlighted = false;

    removed = false;

    currColor = defaultColor;
    // initializes time
    lastPress = std::chrono::high_resolution_clock::now();
}

// sets button default color
// Author: Nathan
void ToggleButton::setDefaultColor(unsigned int color) { defaultColor = color; }
// sets button triggered color
// Author: Nathan
void ToggleButton::setTriggeredColor(unsigned int color) { triggeredColor = color; }
// sets button disabled color
// Author: Nathan
void ToggleButton::setDisabledColor(unsigned int color) { disabledColor = color; }

// draws button
// Author: Nathan
void ToggleButton::drawButton() {
    removed = false;
    LCD.SetFontColor(currColor);
    LCD.DrawRectangle(buttonX - LCD.getCharWidth(), buttonY, buttonWidth + LCD.getCharWidth() * 2, buttonHeight);
    LCD.WriteAt(buttonText, buttonX, buttonY + 4);
}

// redraws button and also updates its state
// Author: Nathan
void ToggleButton::updateButtonState() {
    // update button to fit text
    buttonWidth = buttonText.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    if (enabled) {

        bool withinX = false;
        bool withinY = false;

        // read where cursor is
        LCD.Touch(&touchedX, &touchedY, false);
        // change button highlight
        highlighted = touchedX >= buttonX && touchedX <= buttonX + buttonWidth && touchedY >= buttonY &&
                      touchedY <= buttonY + buttonHeight;

        // update touched location
        if (!LCD.Touch(&touchedX, &touchedY, false)) {

            // wait until touch happens
        } else if (LCD.Touch(&xTrash, &yTrash, false)) {
            // wait until touch releases
            // no joke this is the actual code from FEH documentation -_-
            // if touch is within button boundery then set button state to true
            withinX = touchedX >= buttonX && touchedX <= buttonX + buttonWidth;
            withinY = touchedY >= buttonY && touchedY <= buttonY + buttonHeight;
        }
        if (withinX && withinY) {
            auto currTime = std::chrono::high_resolution_clock::now();
            float debounceTime = 0.1; // debounce time in seconds
            if (std::chrono::duration<float>(currTime - lastPress).count() > debounceTime) {
                if (currState == buttonState::inactive) {
                    currState = buttonState::active;
                } else if (currState == buttonState::active) {
                    currState = buttonState::held;
                }
                currColor = triggeredColor;
                // button is touched
                triggered = !triggered;
                // toggle buttonState
                lastPress = std::chrono::high_resolution_clock::now();
            }
        } else if (!triggered) {
            // no touch
            if (currState == buttonState::held) {
                currState = buttonState::released;
            } else if (currState == buttonState::released) {
                currState = buttonState::inactive;
            }
            // highlight color
            if (highlighted) {
                currColor = highlightedColor;
                if (!wasHighlighted) {
                    // play click sound when highlighted
                    PlayAudioFile("assets/TetrisBlip.wav");
                    wasHighlighted = true;
                }

            } else {
                currColor = defaultColor;
                wasHighlighted = false;
            }
            // else clear touched location to off the screen
            // triggered = false;
            touchedX = numeric_limits<float>::max();
            touchedY = numeric_limits<float>::max();

            // reset buttonState
        }
    }
    // redraws button
    drawButton();
}

// Returns if triggered
// Author: Nathan
bool ToggleButton::getButtonTriggered() { return triggered; }

// Returns true once on click
// Author: Nathan
bool ToggleButton::onButtonClicked() {
    bool clicked = false;
    if (currState == buttonState::active) {
        currState = buttonState::held;
        clicked = true;
    }
    return clicked;
}

// Returns true once on release
// Author: Nathan
bool ToggleButton::onButtonReleased() {
    bool released = false;
    if (currState == buttonState::released) {
        currState = buttonState::inactive;
        released = true;
    }
    return released;
}

// Disables button
// Author: Nathan
void ToggleButton::disable() {
    enabled = false;
    currColor = disabledColor;
}

// Enables button
// Author: Nathan
void ToggleButton::enable() {
    enabled = true;
    currColor = defaultColor;
}

// Sets button as triggered
// Author: Nathan
// Author: Ojas Landge
void ToggleButton::setTriggered(bool t) {
    triggered = t;
    if (t) {
        currState = buttonState::held;
    } else {
        currState = buttonState::released;
    }
}
// Flashes button red
// Author: Nathan
void ToggleButton::flashRed() { currColor = RED; }

// Sets button string
// Author: Nathan
void ToggleButton::setString(std::string s) { buttonText = s; }

// Recenters button
// Author: Nathan
void ToggleButton::recenter() { buttonX = (screenWidth / 2.0) - ((buttonText.length() / 2.0) * LCD.getCharWidth()); }

// Sets x pos
// Author: Nathan
void ToggleButton::setXPos(int x) { buttonX = x; }
// Sets y pos
// Author: Nathan
void ToggleButton::setYPos(int y) { buttonY = y; }
// Gets button string
// Author: Nathan
std::string ToggleButton::getString() { return buttonText; }
// Removes button
// Author: Nathan
void ToggleButton::remove() {
    if (!removed) {
        int backColor = BLACK;
        LCD.SetFontColor(backColor);
        LCD.DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight);
        LCD.WriteAt(buttonText, buttonX, buttonY + 4);
    }
    removed = true;
}
