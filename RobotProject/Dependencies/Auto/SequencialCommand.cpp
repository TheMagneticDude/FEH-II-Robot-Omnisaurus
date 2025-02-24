    #include <limits>
    #include <string>
    #include <FEHXBee.h>
    #include <FEHLCD.h>
    #include "SequencialCommand.h"

    using namespace std;

    //For running commands sequencially
    SequencialCommand::SequencialCommand(){
        currCommand = 0;
        currCommandName = "None";
        finished = false;
        commandInitialized = false;
    }

    void SequencialCommand::addCommand(std::unique_ptr<Command> command){
        //move ownership to the vector from wherever it was first declared so it can be used here 
        commandList.push_back(std::move(command));
    }

    //sequencially runs each command
    void SequencialCommand::runSequencialCommand(){
        unsigned int commandListLen = commandList.size();
        if(currCommand < commandListLen){
            auto &command = commandList[currCommand];
            currCommandName = command->getName();
            if(!command->ended()){
                if(!commandInitialized){
                    //start internal command timer
                    command->init();
                    commandInitialized = true;
                }
                command->run();
            }else{
                //move to next command
                currCommand++;
                commandInitialized = false;
            }
        }else {
            finished = true;
            currCommandName = "None";  // Reset when all commands are done
        }
    }

    std::string SequencialCommand::getCurrentCommandName(){
        return currCommandName;
    }

    //exit condition, returns true once command sequence has ended
    bool SequencialCommand::ended(){return finished;}

