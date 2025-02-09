#pragma once
#include "Grid.h"
#include <FEHLCD.h>
#include <chrono>
#include <string>

#include "Input.h"

using namespace std;

// like normal button class but it stays triggered instead of resetting
class ToggleButton {
  private:
    const unsigned int screenWidth = 320;
    const unsigned int screenHeight = 240;

    const unsigned int defaultNormColor = WHITE;
    const unsigned int defaultTriggeredColor = GRAY;
    const unsigned int defaultDisabledColor = 0x212121;

    enum class buttonState { inactive, active, held, released };

    buttonState currState = buttonState::inactive;

    bool triggered; // if button is being held it is true
    bool enabled;   // if button can be pressed it is true

    int defaultColor;   // normal color of button
    int triggeredColor; // color of button when triggered
    int disabledColor;  // color of button when disabled
    int highlightedColor = BLUEVIOLET;
    bool highlighted;
    bool wasHighlighted = false;

    int currColor;

    std::chrono::high_resolution_clock::time_point lastPress;

  public:
    float buttonX;
    float buttonY;

    float touchedX;
    float touchedY;
    float xTrash;
    float yTrash;

    float buttonCenterX;
    float buttonCenterY;

    float buttonWidth;
    float buttonHeight;

    string buttonText;

    bool removed;

    ToggleButton();
    // creates centered button
    ToggleButton(int y, string text, unsigned int color, unsigned int trigColor);
    ToggleButton(float x, float y, string text);
    ToggleButton(float x, float y, string text, unsigned int color, unsigned int trigColor);

    void drawButton();

    void setDefaultColor(unsigned int color);
    void setTriggeredColor(unsigned int color);
    void setDisabledColor(unsigned int color);
    void updateButtonState();

    bool getButtonTriggered();

    // returns true once when button is pressed and doesent return true again until its been let go and then pressed
    // again
    bool onButtonClicked();

    // returns true once when button is released
    bool onButtonReleased();

    void disable();
    void enable();

    void setTriggered(bool t);

    void flashRed();

    void setString(std::string s);

    void recenter();
    void setXPos(int x);
    void setYPos(int y);
    std::string getString();

    void remove();
};
