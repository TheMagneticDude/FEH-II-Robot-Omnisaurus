#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
// This needs to come after windows.h
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

#if __linux__ && !__ANDROID__
#include <linux/prctl.h>
#include <signal.h>
#include <sys/prctl.h>
#include <unistd.h>
#endif

inline void PlayAudioFile(std::string_view path) {
#ifdef _WIN32
    // Use the windows API for playing sounds (ANSI version)
    PlaySoundA(path.data(), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
#endif
#if __linux__ && !__ANDROID__
    // Use the mplayer command to play sound files
    std::string command = "mplayer ";
    command += path;
    // Pipe the output to /dev/null to not spam the terminal
    command += " &> /dev/null &";
    // Finally, run the command
    system(command.data());
#endif
}

inline void PlayBackgroundMusic() {
    // Tetris song from internet archive: https://ia902905.us.archive.org/11/items/TetrisThemeMusic/Tetris.mp3
#ifdef _WIN32
    // Use Windows MCI (Media Control Interface) to play the background music (this avoids blocking the game, since we
    // can't use PlaySoundA more than once at a time)
    mciSendString(TEXT("open \"assets/TetrisBackground.mp3\" type mpegvideo alias Background"), NULL, 0, NULL);
    mciSendString(TEXT("play Background repeat"), NULL, 0, 0);
#endif
#if __linux__ && !__ANDROID__
    // We will play the sound using a child process so we don't block the game
    pid_t childPid = fork();
    if (childPid == 0) {
        // Forward death signal from the parent to the child process
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        // Close stdout and stderr so we don't spam the terminal
        close(1);
        close(2);
        // Play the music file
        execlp("/usr/bin/mplayer", "mplayer", "assets/TetrisBackground.mp3", NULL);
        // Exec the child process when done
        exit(0);
    }
#endif
}
