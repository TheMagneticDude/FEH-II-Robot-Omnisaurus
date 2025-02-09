#include <limits>
#include <string>

#include "Button.h"
#include "Sound.h"
#include <FEHLCD.h>

using namespace std;

// Author: Nathan
// Constructs a normal buttun using a normal color, and the color for when its triggered
Button::Button(int y, string text, unsigned int color, unsigned int trigColor) {
    buttonX = (screenWidth / 2.0) - ((text.length() / 2.0) * LCD.getCharWidth());
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = color;
    triggeredColor = trigColor;
    disabledColor = defaultDisabledColor;

    currColor = defaultColor;
}

// Author: Nathan
// Creates a buttom that starts out disabled and is centered to act as a display
Button::Button(int y, string text, unsigned int color) {
    buttonX = (screenWidth / 2.0) - ((text.length() / 2.0) * LCD.getCharWidth());
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = false;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = color;
    disabledColor = color;

    currColor = defaultColor;
}

// Author: Nathan
// Construsts a button using the default color and the supplied position.
Button::Button(float x, float y, string text) {
    buttonX = x;
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = defaultNormColor;
    triggeredColor = defaultTriggeredColor;
    disabledColor = defaultDisabledColor;

    currColor = defaultColor;
}

// Author: Nathan
// creates a rectangular button with x and y being the top left of the rectangle
Button::Button(float x, float y, float w, float h, string text) {
    buttonX = x;
    buttonY = y;
    buttonCenterX = x + w / 2;
    buttonCenterY = y + h / 2;
    buttonWidth = w;
    buttonHeight = h;

    buttonText = text;

    triggered = false;
    enabled = true;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = defaultNormColor;
    triggeredColor = defaultTriggeredColor;
    disabledColor = defaultDisabledColor;

    currColor = defaultColor;
}

// Author: Nathan
// creates a rectangular button with x and y being the top left of the rectangle with colors
Button::Button(float x, float y, float w, float h, string text, unsigned int color, unsigned int trigColor) {
    buttonX = x;
    buttonY = y;
    buttonCenterX = x + w / 2;
    buttonCenterY = y + h / 2;
    buttonWidth = w;
    buttonHeight = h;

    buttonText = text;

    triggered = false;
    enabled = true;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = color;
    triggeredColor = trigColor;
    disabledColor = defaultDisabledColor;

    currColor = defaultColor;
}

// Author: Nathan
// Creates a button at a specific position with specific colors
Button::Button(float x, float y, string text, unsigned int color, unsigned int trigColor) {
    buttonX = x;
    buttonY = y;

    buttonWidth = text.length() * LCD.getCharWidth();
    buttonHeight = LCD.getCharHeight();

    buttonText = text;

    triggered = false;
    enabled = true;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = color;
    triggeredColor = trigColor;
    disabledColor = defaultDisabledColor;

    currColor = defaultColor;
}

// initializes button values and whether button starts enabled or disabled
Button::Button(float x, float y, float w, float h, string text, bool e) {
    buttonX = x;
    buttonY = y;
    buttonCenterX = x + w / 2;
    buttonCenterY = y + h / 2;
    buttonWidth = w;
    buttonHeight = h;

    buttonText = text;

    triggered = false;
    enabled = e;
    highlighted = false;
    wasHighlighted = false;

    defaultColor = defaultNormColor;
    triggeredColor = defaultTriggeredColor;
    disabledColor = defaultDisabledColor;
    if (e) {
        currColor = disabledColor;
    } else {
        currColor = defaultColor;
    }
}

void Button::setDefaultColor(unsigned int color) { defaultColor = color; }
void Button::setTriggeredColor(unsigned int color) { triggeredColor = color; }
void Button::setDisabledColor(unsigned int color) { disabledColor = color; }

void Button::drawButton() {
    LCD.SetFontColor(currColor);
    LCD.DrawRectangle(buttonX - LCD.getCharWidth(), buttonY, buttonWidth + LCD.getCharWidth() * 2, buttonHeight);
    LCD.WriteAt(buttonText, buttonX, buttonY + 4);
}

// redraws button and also updates its state
void Button::updateButtonState() {
    if (enabled) {
        bool withinX = false;
        bool withinY = false;

        // read where cursor is
        LCD.Touch(&touchedX, &touchedY, false);
        // change button highlight
        highlighted = touchedX >= buttonX - LCD.getCharWidth() &&
                      touchedX <= buttonX + buttonWidth + LCD.getCharWidth() - 1 && touchedY >= buttonY &&
                      touchedY <= buttonY + buttonHeight;
        // update touched location
        if (!LCD.Touch(&touchedX, &touchedY, false)) {
            // wait until touch happens
        } else if (LCD.Touch(&xTrash, &yTrash, false)) {
            // wait until touch releases
            // no joke this is the actual code from FEH documentation -_-
            // if touch is within button boundery then set button state to true
            withinX =
                touchedX >= buttonX - LCD.getCharHeight() && touchedX <= buttonX + buttonWidth + LCD.getCharWidth() - 1;
            withinY = touchedY >= buttonY && touchedY <= buttonY + buttonHeight;
        }

        if (withinX && withinY) {
            if (currState == buttonState::inactive) {
                currState = buttonState::active;
            } else if (currState == buttonState::active) {
                currState = buttonState::held;
            }
            currColor = triggeredColor;
            // button is touched
            triggered = true;
            // toggle buttonState

        } else {
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
            triggered = false;
            touchedX = numeric_limits<float>::max();
            touchedY = numeric_limits<float>::max();

            // reset buttonState
        }
    }
    // redraws button
    drawButton();
}

bool Button::getButtonTriggered() { return triggered; }

bool Button::onButtonClicked() {
    bool clicked = false;
    if (currState == buttonState::active) {
        currState = buttonState::held;
        clicked = true;
    }
    return clicked;
}

bool Button::onButtonReleased() {
    bool released = false;
    if (currState == buttonState::released) {
        currState = buttonState::inactive;
        released = true;
    }
    return released;
}

void Button::disable() {
    enabled = false;
    currColor = disabledColor;
}

void Button::enable() {
    enabled = true;
    currColor = defaultColor;
}

void Button::setString(std::string s) { buttonText = s; }

void Button::setXPos(int x) { buttonX = x; }
void Button::setYPos(int y) { buttonY = y; }
std::string Button::getString() { return buttonText; }
