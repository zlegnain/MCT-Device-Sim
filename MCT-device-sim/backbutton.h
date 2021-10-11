#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include "button.h"

class BackButton : public Button
{
    Q_OBJECT
public:
    BackButton();
    virtual void sendSignal();

signals:
    void returnToPrevious(); // Sends signal to ControlSystem, handled in backRequested()
};

#endif // BACKBUTTON_H
