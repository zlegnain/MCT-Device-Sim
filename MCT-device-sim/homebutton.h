#ifndef HOMEBUTTON_H
#define HOMEBUTTON_H

#include "button.h"

class HomeButton : public Button
{
    Q_OBJECT
public:
    HomeButton();
    virtual void sendSignal();

signals:
    void returnToMainMenu(); // Sends signal to ControlSystem, handled in homeRequested(int)
};

#endif // HOMEBUTTON_H
