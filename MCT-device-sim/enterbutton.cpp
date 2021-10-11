#include "enterbutton.h"

EnterButton::EnterButton()
{}

void EnterButton::sendSignal()
{
    emit enterRequest();
}
