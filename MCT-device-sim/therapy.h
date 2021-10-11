#ifndef THERAPY_H
#define THERAPY_H

#include <string>

using namespace std;

class Therapy
{
public:
    Therapy(int n);
    virtual ~Therapy();
    virtual string getTitle() = 0;
    virtual string getInstructions() = 0;
    int getFrequencyMode();

protected:
    int frequencyMode;
};

#endif // THERAPY_H
