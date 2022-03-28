#ifndef BUS_BUS_H
#define BUS_BUS_H

#include "Time.h"
#include "Station.h"
#include <iostream>
#include <string>
#include <vector>

typedef std::string String;

class Bus {
public:
    Bus(String lineId, std::vector<int> stationsIds, Time firstDeparture, Time lastDeparture, int interval);
    ~Bus() = default;

    // getters and setters
    String getLineId();
    // std::vector<Station> getStations();
    std::vector<int> getStations();
    Time getFirstDeparture();
    Time getLastDeparture();
    int getInterval();

    void setLineId(int lineId);
    void setStations(const std::vector<int>& stationsIds);
    void setFirstDeparture(const Time& time);
    void setLastDeparture(const Time& time);
    void setInterval(int interval);


private:
    String lineId;
    std::vector<int> stationsIds;
    Time firstDeparture;
    Time lastDeparture;
    int interval;

};


#endif //BUS_BUS_H
