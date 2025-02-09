#pragma once
#include "Grid.h"
#include <FEHImages.h>
#include <FEHLCD.h>
#include <chrono>
#include <string>

using namespace std;

class Animation {
  private:
    bool looping;
    unsigned int startFrame;
    unsigned int currFrame;
    unsigned int maxFrames;
    unsigned int frameTime; // in milliseconds

    int x;
    int y;

    FEHImage image;

    std::chrono::high_resolution_clock::time_point currTime;
    std::chrono::high_resolution_clock::time_point nextFrame;

    std::string fileName;
    std::string fileType;
    std::string fileNumber;
    std::string filePath;

  public:
    Animation(std::string fn, std::string ft, unsigned int fstart, unsigned int fend, unsigned int ftime, int x, int y,
              bool loop);
    void update();
    void replay();
};
