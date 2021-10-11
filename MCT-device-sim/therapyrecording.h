#ifndef THERAPYRECORDING_H
#define THERAPYRECORDING_H

#include <string>
#include <ctime>

using namespace std;

class TherapyRecording
{
public:
    TherapyRecording(string,string,string,int,int);
    string get_treatment();
    string get_date();
    string get_duration();
    int get_freqeuncy();
    int get_powerLevel();

private:
    string treatment;
    string date;
    string duration;
    int frequency;
    int powerLevel;
};

#endif // THERAPYRECORDING_H
