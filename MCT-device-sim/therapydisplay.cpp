#include "therapydisplay.h"
#include "constants.h"

TherapyDisplay::TherapyDisplay()
    : time(0), timeSpent(0), mode(PROGRAM_MODE), active(false)
{

}

void TherapyDisplay::startTherapy(int startTime, int newMode)
{
    time = startTime;
    mode = newMode;
    timeSpent = 0;
    active = true;
}

void TherapyDisplay::endTherapy()
{
    time = 0;
    active = false;
}

void TherapyDisplay::reloadDuration()
{
    timeSpent += 1;
    if (mode == PROGRAM_MODE) {
        if (time <= 0) {
            return;
        }
        time -= 1;
    } else if (mode == FREQUENCY_MODE) {
        time += 1;
    }
}

string TherapyDisplay::getTime()
{
    if (mode == PROGRAM_MODE && time <= 0) {
        active = false;
        return "Program Has Ended";
    }
    string minutes = to_string(time/60);
    if (minutes.length() < 2) {
        minutes = '0'+minutes;
    }
    string seconds = to_string(time%60);
    if (seconds.length() < 2) {
        seconds = '0'+seconds;
    }
    return minutes + "m:" + seconds + "s";
}

int TherapyDisplay::getTimeSpent()
{
    return timeSpent;
}

string TherapyDisplay::getTimeSpent_minutes(){
    string minutes = to_string(timeSpent/60);
    if(minutes.length() < 2){
        minutes = '0'+minutes;
    }
    string seconds = to_string(timeSpent%60);
    if(seconds.length()<2){
        seconds = '0'+seconds;
    }
    return minutes + "m:" + seconds + "s";
}

bool TherapyDisplay::isActive()
{
    return active;
}
