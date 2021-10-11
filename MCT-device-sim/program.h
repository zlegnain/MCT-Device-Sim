#ifndef PROGRAM_H
#define PROGRAM_H

#include "therapy.h"
#include <string>

using namespace std;

class Program : public Therapy
{
public:
    Program(string="", string="", int=0, int=0);
    string getTitle();
    string getProgramMode();
    string getInstructions();
    int getDuration();

private:
    string programMode;
    string instructions;
    int duration; // In seconds
};

#endif // PROGRAM_H
