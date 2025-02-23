#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "SequencialCommand.h"

using namespace std;

//For running commands sequencially
SequencialCommand::SequencialCommand(){currCommand = 0;}

void SequencialCommand::addCommand(std::unique_ptr<Command> command){
    //move ownership to the vector from wherever it was first declared so it can be used here 
    commandList.push_back(std::move(command));
}

//sequencially runs each command
void SequencialCommand::runSequencialCommand(){
    unsigned int commandListLen = commandList.size();
    if(currCommand < commandListLen){
        auto &command = commandList[currCommand];
        if(!command->ended()){
            command->run();
        }else{
            //move to next command
            currCommand++;
        }
    }
}

