#ifndef BATTERY_H
#define BATTERY_H

#include "constants.h"


class Battery
{
public:
    Battery();
    void decreaseBattery(int);
    void increaseBattery();
    int getBatteryLevel();

private:
    int level;
};

#endif // BATTERY_H
