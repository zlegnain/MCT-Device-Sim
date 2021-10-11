#include "frequency.h"

Frequency::Frequency(int n): Therapy(n)
{
}

string Frequency::getTitle()
{
    return "Frequency Mode: "+to_string(getFrequencyMode())+"Hz";
}

string Frequency::getInstructions()
{
    return "Place Electrodes On:\nAffected Area";
}
