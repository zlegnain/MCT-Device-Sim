#include "powerbutton.h"

PowerButton::PowerButton()
{}

void PowerButton::sendSignal()
{
    emit powerRequest();
}
