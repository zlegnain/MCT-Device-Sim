#include "therapy.h"

Therapy::Therapy(int n): frequencyMode(n)
{

}

Therapy::~Therapy()
{

}

int Therapy::getFrequencyMode()
{
    return frequencyMode;
}
