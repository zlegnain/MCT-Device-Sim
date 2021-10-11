#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include <QMainWindow>
#include <QTimer>
#include <string>
#include <iomanip>

#include "constants.h"
#include "menu.h"
#include "settings.h"
#include "therapydisplay.h"
#include "therapyrecording.h"
#include "battery.h"
#include "button.h"
#include "program.h"
#include "frequency.h"

using namespace std;

namespace Ui {
class ControlSystem;
}

class ControlSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlSystem(QWidget *parent = nullptr);
    ~ControlSystem();

private:
    // Methods
    void displayOptions();
    void wipeScreen();
    void selectOption(int);
    void unselectOption(int);
    void displayTherapy(string, string, int);
    void showMsg(string);
    void recordTherapy();
    void clearTherapyRecordings();
    void displayTherapyRecordings();
    void powerToggle();
    void startTherapy(int, int);
    void endTherapy();
    void handleMainMenu();
    void handleSettingsMenu();
    void handleHistoryMenu();
    void handleColourMenu();
    void handleProgramMenu();
    void handleFrequencyMenu();
    void handleChoiceMenu();
    void updateTherapyDisplay(string);
    void decreaseB();
    void increaseB();

    // Initialization
    void initializeState();
    void initializeDevice();
    void initializeMenus();
    void setMenu(int, int, string, string[]);
    void initializeButtons();
    void initializeSensors();
    void initializeTherapies();
    void initializeBatteryTimers();

    Ui::ControlSystem *ui;

    int powerLevel;
    bool power;
    bool skinOn;
    int prevMenu;
    int currMenu;
    int currentSelection;
    int lastTherapy;
    int therapyTerminator;
    unsigned long historyIndex;

    Menu* menus[NUM_MENUS];
    Program* programs[PROGRAM_MENU_LENGTH];
    Frequency* frequencies[FREQUENCY_MENU_LENGTH];
    vector<Button*> buttons;
    vector<TherapyRecording*> recordings;

    TherapyDisplay therapyDisplay;
    Battery battery;
    Settings settings;
    QTimer *therapyTimer;
    QTimer *timer;
    QTimer *timer2;
    bool pIn;
    bool therapyON;

public slots:
    void powerRequested();
    void homeRequested();
    void backRequested();
    void arrowRequested(int);
    void enterRequested();
    void handleTherapyTimer();
    void handleSkinPressed();
    void on_pluggedIn_clicked();

};

#endif // CONTROLSYSTEM_H
