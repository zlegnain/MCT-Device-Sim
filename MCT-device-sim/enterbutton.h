#ifndef ENTERBUTTON_H
#define ENTERBUTTON_H

#include "button.h"

class EnterButton : public Button
{
    Q_OBJECT
public:
    EnterButton();
    virtual void sendSignal();

signals:
    void enterRequest(); // Sends signal to ControlSystem, handled in enterRequested()
};

#endif // ENTERBUTTON_H
