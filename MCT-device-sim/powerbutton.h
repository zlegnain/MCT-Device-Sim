#ifndef POWERBUTTON_H
#define POWERBUTTON_H

#include "button.h"

class PowerButton : public Button
{
    Q_OBJECT
public:
    PowerButton();
    virtual void sendSignal();

signals:
    void powerRequest(); // Sends signal to ControlSystem, handled in powerRequested()
};

#endif // POWERBUTTON_H
