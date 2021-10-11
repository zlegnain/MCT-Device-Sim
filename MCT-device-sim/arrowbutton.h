#ifndef ARROWBUTTON_H
#define ARROWBUTTON_H

#include "button.h"

class ArrowButton : public Button
{
    Q_OBJECT
public:
    ArrowButton(int);
    virtual void sendSignal();

private:
    int direction;

signals:
    void arrowRequest(int); // Sends signal to ControlSystem, handled in arrowRequested()
};

#endif // ARROWBUTTON_H
