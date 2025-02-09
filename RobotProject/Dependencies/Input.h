#pragma once
#include <chrono>
#include <map>
#include <unordered_map>
#include <vector>

#include "Settings.h"

#if __linux__ && !__ANDROID__
#include <X11/Xlib.h>
#endif

class TriggerKey {
  private:
    int keyCode;
    std::chrono::high_resolution_clock::time_point holdStart;
    bool isPressed;
    bool isNewPress;
    bool useDAS;

  public:
    // default constructor
    TriggerKey() : keyCode(0), isPressed(false), isNewPress(false), useDAS(false) {}
    TriggerKey(int keyCode, bool useDAS);
    void update();
    bool newPress();
    bool pressed();
    // Returns the time in seconds
    float holdTime();
    // Simulate a new press and reset the hold timer (used for e.g. DAS)
    void resetHold();

    // change keyCode
    void setKeyCode(int k);
};

class PlayerInput {
  private:
    PlayerHandling *handling;

    bool repeatingLeft;
    bool repeatingRight;

    void handleDAS(bool &isRepeating, TriggerKey &key);

  public:
    // It's in FEH best practices to have all fields be private, but in this case it makes sense for a user of the
    // PlayerInput class to have full access of these, as PlayerInput is a thin wrapper for convenience and organization
    TriggerKey keyLeft;   // 1
    TriggerKey keyRight;  // 2
    TriggerKey rotateCW;  // 3
    TriggerKey rotateCCW; // 4
    TriggerKey rotate180; // 5
    TriggerKey softDrop;  // 6
    TriggerKey hardDrop;  // 7
    TriggerKey swapHold;

    enum class KeyAction { MoveLeft, MoveRight, RotateCW, RotateCCW, Rotate180, SoftDrop, HardDrop, SwapHold };

    std::unordered_map<KeyAction, TriggerKey *> keyBinds;

    std::vector<int> scannedKeys;

    PlayerInput(PlayerSettings &playerSettings);
    // options page
    void setKey(KeyAction key, int keyCode);
    // scans keyboard for a key input and returns the correct int representing that key
    std::vector<int> scanKey();
    void update();
    bool isRepeatingLeft();
    bool isRepeatingRight();
};
