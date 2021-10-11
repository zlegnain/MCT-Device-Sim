#include "button.h"

Button::Button(QObject *parent) : QObject(parent)
{

}

void Button::press()
{
    this->sendSignal();
}
