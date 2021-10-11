#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>

class Button : public QObject
{
    Q_OBJECT
public:
    explicit Button(QObject *parent = nullptr);
    virtual void sendSignal() = 0;

public slots:
    void press(); // Calls sendSignal(), which activates the signals
};

#endif // BUTTON_H
