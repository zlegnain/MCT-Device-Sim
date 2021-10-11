#include "settings.h"

Settings::Settings()
{
    colourPreset = 0;
}

string Settings::getSelectedStyle()
{
    return selectedColourPresets[colourPreset];
}

string Settings::getUnselectedStyle()
{
    return unSelectedColourPresets[colourPreset];
}

void Settings::setColourPreset(int selected)
{
    colourPreset = selected;
}
