#include "homebutton.h"

HomeButton::HomeButton()
{}

void HomeButton::sendSignal()
{
    emit returnToMainMenu();
}
