#include <QVBoxLayout>
#include <QLabel>
#include <QDate>
#include <QTime>

#include "controlsystem.h"
#include "ui_display.h"
#include "constants.h"
#include "arrowbutton.h"
#include "powerbutton.h"
#include "enterbutton.h"
#include "backbutton.h"
#include "homebutton.h"
#include "therapyrecording.h"

ControlSystem::ControlSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlSystem)
{
    ui->setupUi(this);

    initializeState();
    initializeDevice();
}

ControlSystem::~ControlSystem()
{
    // Remove all buttons, etc.
    for (auto i = buttons.begin(); i != buttons.end(); ++i) {
        delete *i;
    }
    for (int i=0; i<NUM_MENUS; i++) {
        delete menus[i];
    }
    for (int i=0; i<PROGRAM_MENU_LENGTH; i++) {
        delete programs[i];
    }
    for (int i=0; i<FREQUENCY_MENU_LENGTH; i++) {
        delete frequencies[i];
    }
    for (auto i = recordings.begin(); i != recordings.end(); ++i) {
        delete *i;
    }
    delete ui;
}

void ControlSystem::displayOptions()
{
    // Fill in screen
    QVBoxLayout *deviceScreen = new QVBoxLayout;

    // find menu object
    // loop thru each option to add as a label
    vector<string> *options = menus[currMenu]->displayOptions();
    for (auto i = options->begin(); i != options->end(); ++i) {
        QLabel *label = new QLabel(tr((*i).c_str()));
        label->setObjectName((*i).c_str());
        label->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
        deviceScreen->addWidget(label);
    }

    wipeScreen();
    ui->groupBox->setLayout(deviceScreen);
    ui->groupBox->setTitle(menus[currMenu]->getName().c_str());
    currentSelection = 0;
    selectOption(currentSelection);
}

void ControlSystem::wipeScreen()
{
    delete ui->groupBox->layout();
    qDeleteAll(ui->groupBox->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
}

void ControlSystem::selectOption(int option)
{
    QList<QObject *> options = ui->groupBox->children();

    options[option+1]->setProperty("styleSheet", settings.getSelectedStyle().c_str());

    qInfo("current selection: %s", qPrintable(options[currentSelection+1]->objectName()));
}

void ControlSystem::unselectOption(int option)
{
    QList<QObject *> options = ui->groupBox->children();
    options[option+1]->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
}

void ControlSystem::powerToggle()
{
    if (battery.getBatteryLevel() < 1){
        power=false;
        qInfo("Can't turn on device as battery is dead");
    }else{
        power = !power;
    }
    wipeScreen();
    if (power) {
        qInfo("Power status: On");
        prevMenu = MAIN_MENU;
        currMenu = MAIN_MENU;
        currentSelection = 0;
        if (!pIn){
            timer->start(1000);
            qInfo("timer started");

        }
        displayOptions();
    } else {
        qInfo("Power  status: Off");
        ui->groupBox->setTitle("");
        ui->batteryDisplay->setText("");
        therapyTimer->stop();
        therapyDisplay.endTherapy();
        endTherapy();
        if (timer->isActive()){
            timer->stop();
            qInfo("timer stopped");
        }

    }
}

void ControlSystem::showMsg(string msg)
{
    currMenu = -1;
    wipeScreen();
    // Fill in screen
    QVBoxLayout *deviceScreen = new QVBoxLayout;

    QLabel *label = new QLabel(tr((msg).c_str()));
    label->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    deviceScreen->addWidget(label);

    wipeScreen();
    ui->groupBox->setLayout(deviceScreen);
    ui->groupBox->setTitle(menus[prevMenu]->getName().c_str());
}

void ControlSystem::handleMainMenu()
{
    if (currentSelection == PROGRAMS_OPTION) {
        currMenu = PROGRAM_MENU;
        displayOptions();
    } else if (currentSelection == FREQUENCY_OPTION) {
        currMenu = FREQUENCY_MENU;
        displayOptions();
    } else if (currentSelection == SETTINGS_OPTION) {
        currMenu = SETTINGS_MENU;
        displayOptions();
    } else if (currentSelection == HISTORY_OPTION) {
        currMenu = HISTORY_MENU;
        displayOptions();
    } else {
        showMsg("Option Not Supported");
    }
}

void ControlSystem::handleSettingsMenu()
{
    if (currentSelection == COLOUR_OPTION) {
        prevMenu = SETTINGS_MENU;
        currMenu = COLOUR_MENU;
        displayOptions();
    } else {
        prevMenu = SETTINGS_MENU;
        showMsg("Setting Not Supported");
    }
}

void ControlSystem::handleHistoryMenu()
{
    currMenu = -1;
    prevMenu = HISTORY_MENU;
    if (currentSelection == VIEW_OPTION) {
        if(recordings.empty()){
            showMsg("No History");
        }else{
            historyIndex = 0;
            displayTherapyRecordings();
        }
    } else if(currentSelection == CLEAR_OPTION){
        clearTherapyRecordings();
    }
}

void ControlSystem::clearTherapyRecordings()
{
    // Deallocate pointers
    for (auto i = recordings.begin(); i != recordings.end(); ++i) {
        delete *i;
    }
    recordings.clear();
    showMsg("History Cleared");
}

void ControlSystem::handleColourMenu()
{
    settings.setColourPreset(currentSelection);
    backRequested();
}

void ControlSystem::handleProgramMenu()
{
    // A program has been selected
    // Start therapy
    int duration = programs[currentSelection]->getDuration(); // In seconds
    startTherapy(duration, PROGRAM_MODE);

    prevMenu = PROGRAM_MENU;
    currMenu = -1;
    // Fill in screen
    displayTherapy(programs[currentSelection]->getTitle(), programs[currentSelection]->getInstructions(), programs[currentSelection]->getFrequencyMode());
}

void ControlSystem::displayTherapyRecordings(){
    //Fill in screen
    QVBoxLayout *Recording_screen = new QVBoxLayout();
    unsigned long end;

    if(recordings.size() % RECORDS_PER_PAGE == 0 || (historyIndex+(RECORDS_PER_PAGE-1)) < recordings.size()){
        end = historyIndex + RECORDS_PER_PAGE;
    }else{
        end = historyIndex + (recordings.size() % RECORDS_PER_PAGE);
    }

    for(unsigned long i =historyIndex;i<end;i++){
        string text = recordings[i]->get_date()+'\n'+
                "Treatment: "+recordings[i]->get_treatment()+'\n'+
                "Frequency: "+to_string(recordings[i]->get_freqeuncy())+"Hz\n"+
                "Power Level: "+to_string(recordings[i]->get_powerLevel())+'\n'+
                "Duration: "+recordings[i]->get_duration();
        QLabel *date_label = new QLabel(tr(text.c_str()));
        date_label->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
        date_label->setAlignment(Qt::AlignLeft);
        Recording_screen->addWidget(date_label);
    }

    unsigned long currPage = (end+RECORDS_PER_PAGE-1)/RECORDS_PER_PAGE;
    unsigned long totalPage = (recordings.size()+RECORDS_PER_PAGE-1)/RECORDS_PER_PAGE;
    string title = "History - Records Page " + to_string(currPage) + "/" + to_string(totalPage);

    wipeScreen();
    ui->groupBox->setLayout(Recording_screen);
    ui->groupBox->setTitle(title.c_str());
}

void ControlSystem::handleFrequencyMenu()
{
    // A frequency mode has been selected
    // Start therapy
    int duration = 0; // In seconds
    startTherapy(duration, FREQUENCY_MODE);

    prevMenu = FREQUENCY_MENU;
    currMenu = -1;
    // Fill in screen
    displayTherapy(frequencies[currentSelection]->getTitle(), frequencies[currentSelection]->getInstructions(), frequencies[currentSelection]->getFrequencyMode());
}

void ControlSystem::displayTherapy(string title, string instructions, int frequency)
{
    // Fill in screen
    QVBoxLayout *deviceScreen = new QVBoxLayout;

    // Instructions
    QLabel *label = new QLabel(tr((instructions).c_str()));
    label->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    label->setAlignment(Qt::AlignCenter);
    deviceScreen->addWidget(label);

    // Frequency
    QLabel *freqLabel = new QLabel(tr(("Frequency: "+to_string(frequency)+"Hz").c_str()));
    freqLabel->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    freqLabel->setAlignment(Qt::AlignCenter);
    deviceScreen->addWidget(freqLabel);

    // Power Level
    QLabel *powLabel = new QLabel(tr(("Power Level: "+to_string(powerLevel)).c_str()));
    powLabel->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    powLabel->setAlignment(Qt::AlignCenter);
    powLabel->setObjectName(POWER_LEVEL_NOTIF);
    deviceScreen->addWidget(powLabel);

    // Skin notif
    string msg = "Electrodes: On";
    if (!skinOn) {
        msg = "Place skin on skin sensor";
    }
    QLabel *skinLabel = new QLabel(tr(msg.c_str()));
    skinLabel->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    skinLabel->setAlignment(Qt::AlignCenter);
    skinLabel->setObjectName(SKIN_NOTIF);
    deviceScreen->addWidget(skinLabel);

    // Timer
    QLabel *timerLabel = new QLabel(tr(""));
    timerLabel->setProperty("styleSheet", settings.getUnselectedStyle().c_str());
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setObjectName(THERAPY_TIMER);
    deviceScreen->addWidget(timerLabel);

    wipeScreen();
    ui->groupBox->setLayout(deviceScreen);
    ui->groupBox->setTitle(title.c_str());
}

void ControlSystem::startTherapy(int duration, int mode)
{
    therapyON = true;
    therapyDisplay.startTherapy(duration, mode);
    if (skinOn) {
        therapyTimer->start(SECOND);
    }
}

void ControlSystem::endTherapy()
{
    if ((therapyTimer->isActive() || therapyDisplay.isActive()) && therapyDisplay.getTimeSpent() > 0) {
        therapyTimer->stop();
        lastTherapy = currentSelection;
        currMenu = CHOICE_MENU;
        displayOptions();
    }
    therapyDisplay.endTherapy();
    therapyON = false;
    qInfo("Power level is back to normal");
}

void ControlSystem::handleChoiceMenu()
{
    if (currentSelection == YES_OPTION) {
        recordTherapy();
    }

    if (therapyTerminator == HOME_BUTTON){
        homeRequested();
    } else {
        backRequested();
    }
}

void ControlSystem::recordTherapy(){
    QString date = QDate::currentDate().toString();
    QString time = QTime::currentTime().toString();
    qInfo("here");
    if(therapyDisplay.getTimeSpent() != 0){
        if(prevMenu == PROGRAM_MENU){
            recordings.push_back(new TherapyRecording(programs[lastTherapy]->getProgramMode(), (date+" "+time).toUtf8().constData(), therapyDisplay.getTimeSpent_minutes(), frequencies[lastTherapy]->getFrequencyMode(),powerLevel));
        }else if (prevMenu == FREQUENCY_MENU){
            recordings.push_back(new TherapyRecording("Frequency Mode", (date+" "+time).toUtf8().constData(), therapyDisplay.getTimeSpent_minutes(), frequencies[lastTherapy]->getFrequencyMode(),powerLevel));
        }
    }
}

void ControlSystem::updateTherapyDisplay(string updatedTime)
{
    QLabel *label = ui->groupBox->findChild<QLabel *>(THERAPY_TIMER);
    if (label) {
        label->setText(updatedTime.c_str());
    }
}

void ControlSystem::powerRequested()
{
    qInfo("power request");
    endTherapy();
    powerToggle();
}

void ControlSystem::homeRequested()
{
    if (!power) {
        return;
    }
    if ((therapyTimer->isActive() || therapyDisplay.isActive()) && therapyDisplay.getTimeSpent() > 0) {
        therapyTerminator = HOME_BUTTON;
        endTherapy();
    }else{
        prevMenu = MAIN_MENU;
        currMenu = MAIN_MENU;
        displayOptions();
        qInfo("home request");
    }
}

void ControlSystem::backRequested()
{
    if (!power) {
        return;
    }
    if ((therapyTimer->isActive() || therapyDisplay.isActive()) && therapyDisplay.getTimeSpent() > 0) {
        therapyTerminator = BACK_BUTTON;
        endTherapy();
    }else{
        if (currMenu != MAIN_MENU && currMenu == prevMenu) {
            homeRequested();
            return;
        }
        qInfo("back request");
        currMenu = prevMenu;
        displayOptions();
    }
}

void ControlSystem::arrowRequested(int direction)
{
    if (!power) {
        return;
    }
    qInfo("arrow request: direction %d", direction);
    if (currMenu >= 0) {
        unselectOption(currentSelection);
        if (direction == UP) {
            currentSelection -= 1;
            if (currentSelection < 0) { // Bring to end
                currentSelection = menus[currMenu]->getSize()-1;
            }
        } else if (direction == DOWN) {
            currentSelection += 1;
            if (currentSelection > menus[currMenu]->getSize()-1) {
                currentSelection = 0;  // Bring to beginning
            }
        }
        selectOption(currentSelection);
        return;
    } else {
        if(prevMenu == PROGRAM_MENU || prevMenu == FREQUENCY_MENU){
            if (!therapyDisplay.isActive()) {
                return;
            }
            if (direction == LEFT) {
                powerLevel -= 10;
                if (powerLevel < 10) {
                    powerLevel = 10;
                }
                qInfo("Power level has decreased to %d", powerLevel);
            } else if (direction == RIGHT) {
                powerLevel += 10;
                if (powerLevel > 100) {
                    powerLevel = 100;
                }
                qInfo("Power level has increased to %d", powerLevel);
            }
            QLabel *label = ui->groupBox->findChild<QLabel *>(POWER_LEVEL_NOTIF);
            if (label) {
                label->setText(("Power Level: "+to_string(powerLevel)).c_str());
            }
        }else if(prevMenu == HISTORY_MENU && !recordings.empty()){
            if (direction == LEFT) {
                if(historyIndex != 0){
                    historyIndex -= RECORDS_PER_PAGE;
                }else{
                    while(historyIndex < recordings.size()){
                        historyIndex += RECORDS_PER_PAGE;
                    }
                    historyIndex -= RECORDS_PER_PAGE;
                }
            } else if (direction == RIGHT) {
                historyIndex += RECORDS_PER_PAGE;
                if (historyIndex >= recordings.size()) {
                    historyIndex = 0;
                }
            }
            displayTherapyRecordings();
        }
    }
}

void ControlSystem::enterRequested()
{
    if (!power) {
        return;
    }
    if (currMenu == MAIN_MENU) {
        handleMainMenu();
    } else if (currMenu == SETTINGS_MENU) {
        handleSettingsMenu();
    } else if (currMenu == COLOUR_MENU) {
        handleColourMenu();
    } else if (currMenu == PROGRAM_MENU) {
        handleProgramMenu();
    } else if (currMenu == FREQUENCY_MENU) {
        handleFrequencyMenu();
    } else if (currMenu == HISTORY_MENU) {
        handleHistoryMenu();
    } else if (currMenu == CHOICE_MENU) {
        handleChoiceMenu();
    }
    qInfo("enter request: option %d", currentSelection);
}

void ControlSystem::handleTherapyTimer()
{
    string currTime = therapyDisplay.getTime(); // display this too
    updateTherapyDisplay(currTime);
    therapyDisplay.reloadDuration();

    if (!therapyDisplay.isActive()) {
        therapyTerminator = TIME_OUT;
        endTherapy();
    }
}

void ControlSystem::handleSkinPressed()
{
    skinOn = !skinOn;
    QLabel *label = ui->groupBox->findChild<QLabel *>(SKIN_NOTIF);
    if (skinOn) {
        ui->skin_toggle->setText("Skin: On");
        if (label) {
            label->setText("Electrodes: On");
        }
        therapyTimer->stop();
        if (therapyDisplay.isActive() && !therapyTimer->isActive()) {
            therapyTimer->start(SECOND);
        }
    } else {
        ui->skin_toggle->setText("Skin: Off");
        if (label) {
            label->setText("Electrodes: Off");
        }
        if (therapyTimer->isActive()) {
            therapyTimer->stop();
        }
    }
}

void ControlSystem::initializeState()
{
    power = false;
    powerLevel = 100;
    skinOn = false;
    pIn = false;
    therapyON = false;
    prevMenu = MAIN_MENU;
    currMenu = MAIN_MENU;
    currentSelection = 0;
}

void ControlSystem::initializeDevice()
{
    initializeMenus();
    initializeButtons();
    initializeSensors();
    initializeTherapies();
    initializeBatteryTimers();
}

void ControlSystem::initializeMenus()
{
    // Set main menu
    string main_menu_options[MAIN_MENU_LENGTH] = {"Programs", "Frequencies", "MED", "Screening", "Children", "Settings", "Safety","History"};
    setMenu(MAIN_MENU, MAIN_MENU_LENGTH, "Main Menu", main_menu_options);

    // Set program menu
    string program_menu_options[PROGRAM_MENU_LENGTH] = {"Bloating", "Gynecologic Pain", "Hypotonia", "Trauma"};
    setMenu(PROGRAM_MENU, PROGRAM_MENU_LENGTH, "Programs", program_menu_options);

    // Set frequency menu
    string frequency_menu_options[FREQUENCY_MENU_LENGTH] = {"10 Hz", "20 Hz", "60 Hz", "77 Hz", "140 Hz"};
    setMenu(FREQUENCY_MENU, FREQUENCY_MENU_LENGTH, "Frequencies", frequency_menu_options);

    // Set settings menu
    string settings_menu_options[SETTINGS_MENU_LENGTH] = {"Brightness", "Economy", "Recording", "Clock", "Alarm Clock", "Language", "Colour"};
    setMenu(SETTINGS_MENU, SETTINGS_MENU_LENGTH, "Settings", settings_menu_options);

    // Set colour menu
    string colour_menu_options[COLOUR_MENU_LENGTH] = {"Default", "Turquiose", "Winter", "Bronze", "Night"};
    setMenu(COLOUR_MENU, COLOUR_MENU_LENGTH, "Colour", colour_menu_options);

    // Set history menu
    string history_menu_options[HISTORY_MENU_LENGTH] = {"View History", "Clear History"};
    setMenu(HISTORY_MENU, HISTORY_MENU_LENGTH, "History", history_menu_options);

    // Set choice menu
    string choice_menu_options[CHOICE_MENU_LENGTH] = {"Yes", "No"};
    setMenu(CHOICE_MENU, CHOICE_MENU_LENGTH, "Record Therapy?", choice_menu_options);
}

void ControlSystem::setMenu(int menu, int menu_length, string menu_name, string options[])
{
    menus[menu] = new Menu(menu_name);
    for (int i=0; i<menu_length; i++) {
        menus[menu]->addOption(options[i]);
    }
}

void ControlSystem::initializeButtons()
{
    int directions[4] = {UP, DOWN, RIGHT, LEFT};
    QPushButton* directionButtons[4] = {ui->upButton, ui->downButton, ui->rightButton, ui->leftButton};
    for (int i=0; i<4; i++) {
        Button* arrowButton = new ArrowButton(directions[i]);
        buttons.push_back(arrowButton);
        connect(directionButtons[i], SIGNAL(released()), arrowButton, SLOT(press()));
        connect(arrowButton, SIGNAL(arrowRequest(int)), this, SLOT(arrowRequested(int)));
    }

    Button* homeButton = new HomeButton();
    buttons.push_back(homeButton);
    connect(ui->homeButton, SIGNAL(released()), homeButton, SLOT(press()));
    connect(homeButton, SIGNAL(returnToMainMenu()), this, SLOT(homeRequested()));

    Button* backButton = new BackButton();
    buttons.push_back(backButton);
    connect(ui->backButton, SIGNAL(released()), backButton, SLOT(press()));
    connect(backButton, SIGNAL(returnToPrevious()), this, SLOT(backRequested()));

    Button* powerButton = new PowerButton();
    buttons.push_back(powerButton);
    connect(ui->powerButton, SIGNAL(released()), powerButton, SLOT(press()));
    connect(powerButton, SIGNAL(powerRequest()), this, SLOT(powerRequested()));

    Button* enterButton = new EnterButton();
    buttons.push_back(enterButton);
    connect(ui->enterButton, SIGNAL(released()), enterButton, SLOT(press()));
    connect(enterButton, SIGNAL(enterRequest()), this, SLOT(enterRequested()));

    // Buttons
    int d = qMin(ui->homeButton->height(), ui->homeButton->width()) - 2 ;
    int xOffset =(ui->homeButton->width() - d ) / 2;
    int yOffset =(ui->homeButton->height() - d) / 2;

    QRect rect(xOffset,yOffset,d,d);
    QRegion region(rect, QRegion::Ellipse);

    ui->homeButton->setIcon(QIcon((QPixmap("../pics/home_button.png"))));
    ui->homeButton->setIconSize(QSize(25, 25));
    ui->homeButton->setMask(region);


    ui->backButton->setIcon(QIcon((QPixmap("../pics/back_button.png"))));
    ui->backButton->setIconSize(QSize(25, 25));
    ui->backButton->setMask(region);

    ui->powerButton->setIcon(QIcon((QPixmap("../pics/power_button.png"))));
    ui->powerButton->setIconSize(QSize(25, 25));
    ui->powerButton->setMask(region);

    ui->enterButton->setMask(region);

    ui->upButton->setIcon(QIcon((QPixmap("../pics/up_button.png"))));
    ui->upButton->setIconSize(QSize(25, 25));
    ui->upButton->setMask(region);

    QTransform tr;
    tr.rotate(90);

    ui->rightButton->setIcon(QIcon((QPixmap("../pics/up_button.png").transformed(tr))));
    ui->rightButton->setIconSize(QSize(25, 25));
    ui->rightButton->setMask(region);

    tr.rotate(90);
    ui->downButton->setIcon(QIcon((QPixmap("../pics/up_button.png").transformed(tr))));
    ui->downButton->setIconSize(QSize(25, 25));
    ui->downButton->setMask(region);

    tr.rotate(90);
    ui->leftButton->setIcon(QIcon((QPixmap("../pics/up_button.png").transformed(tr))));
    ui->leftButton->setIconSize(QSize(25, 25));
    ui->leftButton->setMask(region);
}

void ControlSystem::initializeSensors()
{
    skinOn = false;
    ui->skin_toggle->setText("Skin: Off");
    connect(ui->skin_toggle, SIGNAL(released()), this, SLOT(handleSkinPressed()));
}

void ControlSystem::initializeTherapies()
{
    // Initialize programs and frequency modes
    string program_modes[PROGRAM_MENU_LENGTH] = {"Bloating", "Gynecologic Pain", "Hypotonia", "Trauma"};
    string program_instructions[PROGRAM_MENU_LENGTH] = {"Front Abdominal Wall", "Painful Zone of Stomach",
                                                        "Neck and Collar Zone", "Affected Zone"};
    int program_frequency[PROGRAM_MENU_LENGTH] = {77, 77, 77, 200};
    int program_duration[PROGRAM_MENU_LENGTH] = {600, 900, 300, 900}; // In minutes: 10m, 15m, 5m, 15m
    for (int i=0;i<PROGRAM_MENU_LENGTH;i++) {
        programs[i] = new Program(program_modes[i], program_instructions[i], program_frequency[i], program_duration[i]);
    }

    int frequency[FREQUENCY_MENU_LENGTH] = {10, 20, 60, 77, 140};
    for (int i=0;i<FREQUENCY_MENU_LENGTH;i++) {
        frequencies[i] = new Frequency(frequency[i]);
    }

    // Initialize device therapy program
    therapyTimer = new QTimer(this);
    connect(therapyTimer,&QTimer::timeout,[this](){ handleTherapyTimer(); });
}

void ControlSystem::initializeBatteryTimers(){
    timer = new QTimer(this); //create timer with optional parent object
    connect(timer,&QTimer::timeout,[this](){ decreaseB(); });
    timer2 = new QTimer(this); //create timer with optional parent object
    connect(timer2,&QTimer::timeout,[this](){ increaseB(); });
}

void ControlSystem::decreaseB()
{
    QString time = QTime::currentTime().toString().left(5) + " - ";
    if (battery.getBatteryLevel() < 1){
        ui->batteryDisplay->setText(time);
        qInfo("Current Battery Level: %d", battery.getBatteryLevel());
        timer->stop();
        qInfo("timer stopped");
        therapyON = false;
        powerToggle();
        return;
    }
    ui->batteryDisplay->setText(time+(to_string(battery.getBatteryLevel())+"%").c_str());
    qInfo("Current Battery Level: %d", battery.getBatteryLevel());
    if (therapyON && skinOn){
        qInfo("newUnit: %d", (powerLevel/10));
        battery.decreaseBattery((powerLevel/10));
    }
    else{
       battery.decreaseBattery(UNIT);
    }
}

void ControlSystem::increaseB()
{
    QString time = QTime::currentTime().toString().left(5) + " - ";
    if (battery.getBatteryLevel() > 99){
        ui->batteryDisplay->setText(time+(to_string(battery.getBatteryLevel())+"%").c_str());
        qInfo("Current Battery Level: %d", battery.getBatteryLevel());
        timer2->stop();
        qInfo("timer2 stopped");
        return;
    }
    ui->batteryDisplay->setText(time+(to_string(battery.getBatteryLevel())+"%").c_str());
    qInfo("Current Battery Level: %d", battery.getBatteryLevel());
    battery.increaseBattery();
}

void ControlSystem::on_pluggedIn_clicked()
{
    if (ui->pluggedIn->isChecked()){
          QString time = QTime::currentTime().toString().left(5) + " - ";
          ui->batteryDisplay->setText(time+(to_string(battery.getBatteryLevel())+"%").c_str());
          pIn = true;
          qInfo("Device is Plugged In");
          if (timer->isActive()){
              timer->stop();
              qInfo("timer stopped");
          }
          if (battery.getBatteryLevel()<100){
              timer2->start(1000);
              qInfo("timer2 started");
          }

    }
    else{
        pIn = false;
        qInfo("Device is not Plugged In");
        if (timer2->isActive()){
             timer2->stop();
             qInfo("timer2 stopped");
        }
        if (!power) {
            ui->batteryDisplay->setText("");
        }
        if (power && battery.getBatteryLevel()>0){
            timer->start(1000);
            qInfo("timer2 started");
        }

    }
}
