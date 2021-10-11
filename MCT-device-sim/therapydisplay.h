#ifndef THERAPYDISPLAY_H
#define THERAPYDISPLAY_H

#include <string>

using namespace std;

class TherapyDisplay
{
public:
    TherapyDisplay();
    void startTherapy(int, int);
    void endTherapy();
    void reloadDuration();
    string getTime();
    int getTimeSpent();
    bool isActive();
    string getTimeSpent_minutes();

private:
    int time; // Seconds
    int timeSpent;
    int mode;
    bool active;
};

#endif // THERAPYDISPLAY_H
