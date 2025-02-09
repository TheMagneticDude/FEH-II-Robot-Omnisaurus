#include <unordered_map>
#include <vector>

#include <chrono>

#include "Input.h"
#include "Settings.h"
#include <FEHLCD.h>

#ifdef _WIN32
#include <windows.h>
#endif

#if __linux__ && !__ANDROID__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <tigr.h>

// Author: Ojas
// Get the display handle to be used for keyboard input on Linux
Display *getX11Display() {
    // my guy you are a madman
    // I'm sorry but it had to be done :)
    // The TigrInternal structure is placed directly after Tigr (Tigr* is at offset 0 in LCD)
    Tigr *internal = *reinterpret_cast<Tigr **>(&LCD) + 1;
    // The Display* is at offset 48 in TigrInternal
    uint8_t *disp_ptr = reinterpret_cast<uint8_t *>(internal) + 48;
    // Now we have it
    Display *display = *reinterpret_cast<Display **>(disp_ptr);
    return display;
}
#endif

// Author: Ojas
// Construct the key logic for a specific key on the keyboard
TriggerKey::TriggerKey(int keyCode, bool useDAS = false)
    : keyCode(keyCode), useDAS(useDAS), isPressed(false), isNewPress(false) {}

// Author: Nathan
// Set the internal keyCode for when a setting is changed
void TriggerKey::setKeyCode(int k) { keyCode = k; }

// Author: Ojas
// Update the state of the key (check to see if it is pressed, and repeat the input if DAS is in use)
void TriggerKey::update() {
    bool currentlyIsPressed;

#ifdef _WIN32
    currentlyIsPressed = GetAsyncKeyState(keyCode) & 0x8000;

#endif

#if __linux__ && !__ANDROID__
    Display *display = getX11Display();
    char keymap[32];
    XQueryKeymap(display, keymap);
    int xKeyCode = XKeysymToKeycode(display, keyCode);
    // Extract the bit that corresponds to the specific key
    currentlyIsPressed = keymap[xKeyCode / 8] & (1 << (xKeyCode % 8));
#endif

    if (currentlyIsPressed && !isPressed) {
        isPressed = true;
        resetHold();
    } else if (!currentlyIsPressed && isPressed) {
        isPressed = false;
    }
}

// Author: Ojas
// Check to see if the key has been pressed since the last time this function was called
bool TriggerKey::newPress() {
    bool wasNewPress = isNewPress;
    isNewPress = false;
    return wasNewPress;
}

// Author: Ojas
// Check to see if the key is currently pressed
bool TriggerKey::pressed() { return isPressed; }

// Author: Ojas
// Check how long the key has been held for
float TriggerKey::holdTime() {
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - holdStart;
    return duration.count();
}

// Author: Ojas
// Reset the key state in the scenario of a new key press (real or simulated)
void TriggerKey::resetHold() {
    isNewPress = true;
    holdStart = std::chrono::high_resolution_clock::now();
}

// Author: Ojas and Nathan
// Construct the player input class by constructing each key using the user's settings
PlayerInput::PlayerInput(PlayerSettings &playerSettings)
    : handling(&playerSettings.handling), keyLeft(playerSettings.controls.moveLeft, true),
      keyRight(playerSettings.controls.moveRight, true), rotateCW(playerSettings.controls.rotateCW),
      rotateCCW(playerSettings.controls.rotateCCW), rotate180(playerSettings.controls.rotate180),
      softDrop(playerSettings.controls.softDrop), hardDrop(playerSettings.controls.hardDrop),
      swapHold(playerSettings.controls.swapHold) {

    // create keymap

    keyBinds.emplace(KeyAction::MoveLeft, &keyLeft);
    keyBinds.emplace(KeyAction::MoveRight, &keyRight);
    keyBinds.emplace(KeyAction::RotateCW, &rotateCW);
    keyBinds.emplace(KeyAction::RotateCCW, &rotateCCW);
    keyBinds.emplace(KeyAction::Rotate180, &rotate180);
    keyBinds.emplace(KeyAction::SoftDrop, &softDrop);
    keyBinds.emplace(KeyAction::HardDrop, &hardDrop);
    keyBinds.emplace(KeyAction::SwapHold, &swapHold);
}

// Author: Ojas
// Construct the player input class by constructing each key using the user's settings
void PlayerInput::update() {
    keyLeft.update();
    keyRight.update();
    rotateCW.update();
    rotateCCW.update();
    rotate180.update();
    softDrop.update();
    hardDrop.update();
    swapHold.update();

    handleDAS(repeatingLeft, keyLeft);
    handleDAS(repeatingRight, keyRight);
}

// Author: Nathan
// Change a specific key binding in the case of a new key mapping
void PlayerInput::setKey(KeyAction key, int keyCode) { keyBinds[key]->setKeyCode(keyCode); }

// Author: Nathan
// Check to see if any keys are currently pressed (used when remapping keys)
// Windows scanKey will figure out linux later
#ifdef _WIN32
// scans keys and stores them into a vector
std::vector<int> PlayerInput::scanKey() {
    scannedKeys.clear();
    // Iterate through possible virtual key codes (0x20 to 0xFE) (spacebar to clear key)
    for (int key = 0x20; key <= 0xFE; key++) {
        // check if the key is pressed
        // 0x8000 is doing a bitwise and operation on what asyncjkey returns so it basically makes sure key is being
        // pressed
        if (GetAsyncKeyState(key) & 0x8000) {
            // adds key to pressedKeys
            scannedKeys.push_back(key);
        }
    }
    return scannedKeys;
}
#endif

#if __linux__ && !__ANDROID__
std::vector<int> PlayerInput::scanKey() {
    scannedKeys.clear();

    Display *display = getX11Display();
    char keymap[32];
    XQueryKeymap(display, keymap);

    int allowedKeys[] = {XK_a,     XK_b,  XK_c,    XK_d,     XK_e,     XK_f,       XK_g,     XK_h,    XK_i,
                         XK_j,     XK_k,  XK_l,    XK_m,     XK_n,     XK_o,       XK_p,     XK_q,    XK_r,
                         XK_s,     XK_t,  XK_u,    XK_v,     XK_w,     XK_x,       XK_y,     XK_z,    XK_Left,
                         XK_Right, XK_Up, XK_Down, XK_comma, XK_space, XK_Shift_L, XK_Alt_L, XK_Alt_R};

    // Extract the bit that corresponds to the specific key
    for (int i = 0; i < sizeof(allowedKeys) / sizeof(int); i++) {
        int xKeyCode = XKeysymToKeycode(display, allowedKeys[i]);
        bool currentlyIsPressed = keymap[xKeyCode / 8] & (1 << (xKeyCode % 8));
        if (currentlyIsPressed) {
            scannedKeys.push_back(allowedKeys[i]);
        }
    }

    // This is not yet supported on Linux, but does not affect the functionality of the game
    return scannedKeys;
}
#endif

// Author: Ojas
// This function implement DAS (Delayed Auto Shift) logic by simulated a new press when the hold time is greater than
// the DAS setting.
void PlayerInput::handleDAS(bool &isRepeating, TriggerKey &key) {
    if (!key.pressed() && isRepeating) {
        isRepeating = false;
    } else if (isRepeating && key.holdTime() > handling->arr) {
        key.resetHold();
    } else if (!repeatingLeft && key.pressed() && key.holdTime() > handling->das) {
        key.resetHold();
        isRepeating = true;
    }
}

bool PlayerInput::isRepeatingLeft() { return repeatingLeft; }
bool PlayerInput::isRepeatingRight() { return repeatingRight; }