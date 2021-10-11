#include "backbutton.h"

BackButton::BackButton()
{}

void BackButton::sendSignal()
{
    emit returnToPrevious();
}
