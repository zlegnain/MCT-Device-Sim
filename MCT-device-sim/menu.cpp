#include "menu.h"

Menu::Menu(string n)
    : menuName(n), size(0)
{}

string Menu::getName()
{
    return this->menuName;
}

int Menu::getSize()
{
    return this->size;
}

vector<string>* Menu::displayOptions()
{
    return &options;
}


void Menu::addOption(string option)
{
    options.push_back(option);
    size += 1;
}
