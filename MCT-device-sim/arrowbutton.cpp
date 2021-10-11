#include "arrowbutton.h"

ArrowButton::ArrowButton(int d)
    : direction(d)
{}

void ArrowButton::sendSignal()
{
    emit this->arrowRequest(this->direction);
}
