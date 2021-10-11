#include "program.h"

Program::Program(string a, string b, int c, int d)
    : Therapy(c), programMode(a), instructions(b), duration(d)
{

}

string Program::getTitle()
{
    return "Program:"+getProgramMode();
}

int Program::getDuration()
{
    return duration;
}


string Program::getProgramMode()
{
    return programMode;
}

string Program::getInstructions()
{
    return "Place Electrodes On:\n"+instructions;
}
