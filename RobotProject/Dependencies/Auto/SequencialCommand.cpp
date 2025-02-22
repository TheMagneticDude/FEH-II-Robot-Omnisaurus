#include <limits>
#include <string>
#include <FEHXBee.h>
#include <FEHLCD.h>
#include "SequencialCOmmand.h"

using namespace std;

//For running commands sequencially
SequencialCommand::SequencialCommand(HolonomicTriangleDrive &dt) : drivetrain(dt){}

void SequencialCommand::addCommand(Command command){
    commandList.push_back(command);
}

void SequencialCommand::runSequencialCommand(){
    for(Command command : commandList){
        while(!command.ended()){
            command.run();
        }
    }
}

