#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "therapy.h"


class Frequency : public Therapy
{
public:
    Frequency(int=0);
    string getTitle();
    string getInstructions();
};

#endif // FREQUENCY_H
