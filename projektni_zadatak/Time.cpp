#include "Time.h"

Time::Time(int hours, int minutes) {
    setTime(hours, minutes);
}

Time::Time(String& time) {
    // hh:mm
    String hh;
    String mm;
    for (int i = 0; i < 2; i++) {
        hh.push_back(time[i]);
    }
    for (int i = 3; i < 5; i++) {
        mm.push_back(time[i]);
    }
    this->hours = std::stoi(hh);
    this->minutes = std::stoi(mm);
}

int Time::getHours() {
    return this->hours;
}

int Time::getMinutes() {
    return this->minutes;
}

void Time::setTime(int hours, int minutes) {
    if (hours >= 0 && hours < 24) {
        this->hours = hours;
    }
    else throw "Wrong time";
    if (minutes >= 0 && hours < 60) {
        this->minutes = minutes;
    }
    else throw "Wrong time";
}

String Time::getTime() {
    String time;
    String h = std::to_string(this->hours);
    String m = std::to_string(this->minutes);
    if (h.size() == 1) {
        time.push_back('0');
    }
    time.append(h);
    time.push_back(':');
    if (m.size() == 1) {
        time.push_back('0');
    }
    time.append(m);
    return time;
}

void Time::addMinutes(int min) {
    this->minutes += min;
    if (this->minutes >= 60) {
        this->hours++;
        this->minutes -= 60;
    }
}


