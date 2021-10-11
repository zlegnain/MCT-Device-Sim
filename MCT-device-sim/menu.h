#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

using namespace std;

class Menu
{
public:
    Menu(string="Unnamed");
    vector<string>* displayOptions();
    string getName();
    int getSize();
    void addOption(string);

private:
    string menuName;
    int size;
    vector<string> options;
};

#endif // MENU_H
