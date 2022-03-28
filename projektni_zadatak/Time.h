#ifndef BUS_TIME_H
#define BUS_TIME_H

#include <iostream>
#include <string>
#include <vector>

typedef std::string String;

class Time {
public:
    Time(int hours, int minutes);
    // loads time from string in format hh:mm
    Time(String& time);

    int getHours();

    int getMinutes();

    void setTime(int hours, int minutes);

    // returns time in (hh:mm) format
    String getTime();

    // add minutes to time
    void addMinutes(int min);

private:
    int hours;
    int minutes;

};


#endif //BUS_TIME_H
