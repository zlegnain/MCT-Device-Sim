#include "therapyrecording.h"

TherapyRecording::TherapyRecording(string t, string d, string du, int f, int p):
    treatment(t), date(d),duration(du),frequency(f),powerLevel(p)
{

}

string TherapyRecording::get_treatment(){
    return treatment;
}

string TherapyRecording::get_date(){
    return date;
}

string TherapyRecording::get_duration(){
    return duration;
}

int TherapyRecording::get_freqeuncy(){
    return frequency;
}

int TherapyRecording::get_powerLevel(){
    return powerLevel;
}
