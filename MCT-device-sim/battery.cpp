#include "battery.h"


Battery::Battery(): level(100)
{

}

void Battery::decreaseBattery(int powerLevel)
{
    if (level-powerLevel<0){
        level=0;
    }
    else{
        level -= powerLevel;
    }


}

void Battery::increaseBattery()
{
    if ((level+(5*UNIT)) >100){
        level = 100;
    }
    else{
        level +=  5*UNIT;
    }

}


int Battery::getBatteryLevel(){
    return level;
}
