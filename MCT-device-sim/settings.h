#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
using namespace std;


class Settings
{
public:
    Settings();
    string getSelectedStyle();
    string getUnselectedStyle();
    void setColourPreset(int);

private:
    int colourPreset;
    string unSelectedColourPresets[5] = {"QLabel { background-color : ; color : ; }", // Default
                                         "QLabel { background-color : ; color : blue; }", // Turquiose
                                         "QLabel { background-color : ; color : blue; }", // Winter
                                         "QLabel { background-color : ; color : blue; }", // Bronze
                                         "QLabel { background-color : black; color : white; }"}; // Night
    string selectedColourPresets[5] = {"QLabel { background-color : black; color : white; }",
                                       "QLabel { background-color : blue; color : white; }",
                                       "QLabel { background-color : green; color : white; }",
                                       "QLabel { background-color : red; color : white; }",
                                       "QLabel { background-color : ; color : ; }"}; // Text and background pairs mapped to colour presets
};

#endif // SETTINGS_H
