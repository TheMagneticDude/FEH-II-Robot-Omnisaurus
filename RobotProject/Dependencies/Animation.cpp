#include "Animation.h"
#include <FEHLCD.h>
#include <cmath>
#include <math.h>
#include <string>

using namespace std;

// Initializes the animation
// Author: Nathan Cheng
Animation::Animation(std::string fn, std::string ft, unsigned int fstart, unsigned int fend, unsigned int ftime, int xP,
                     int yP, bool loop) {
    x = xP;
    y = yP;
    fileNumber = to_string(currFrame);

    fileName = fn;
    fileType = ft;
    startFrame = fstart;
    currFrame = startFrame;
    maxFrames = fend;
    frameTime = ftime;

    fileNumber = to_string(currFrame);
    filePath = fileName + fileNumber + fileType;
    image.Open(filePath.c_str());

    looping = loop;
    currTime = std::chrono::high_resolution_clock::now();
}

// Updates the animation frames given the inputs
// Author: Nathan
void Animation::update() {
    currTime = std::chrono::high_resolution_clock::now();
    auto timeToNextFrame = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - nextFrame);
    if (timeToNextFrame.count() >= frameTime && currFrame < maxFrames) {
        image.Close();
        nextFrame = currTime + std::chrono::milliseconds(frameTime);
        currFrame++;

        // update text
        fileNumber = to_string(currFrame);
        filePath = fileName + fileNumber + fileType;
        image.Open(filePath.c_str());
    }

    image.Draw(x, y);

    if (looping) {
        if (currFrame >= maxFrames) {
            // keep looping
            currFrame = startFrame;
        }
    }
}

// Replays the animation from the beginning
// Author: Nathan
void Animation::replay() { currFrame = startFrame; }
