#ifndef BUS_SYSTEM_H
#define BUS_SYSTEM_H

#include "Bus.h"
#include "Exceptions.h"
#include "Graph.h"
#include "Station.h"
#include "Time.h"
#include "Loader.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

typedef std::string String;

class System {
public:
    System(const String& stations_file, const String& buses_file);
    ~System() = default;

    // load Stations and Buses from input files
    void load(const String& stations_file, const String& buses_file);

    // INFO
    void getStationInfo(int id);

    void getBusInfo(const String& line);

    void statistics(const String& line);

    // ROUTES
    void getRoute(int start, int end, Time time);

private:
    std::vector<Station> stations;
    std::vector<Bus> buses;

    // private methods
    Station findStationById(int id);

    Bus findBusById(const String& line);

    std::vector<String> getBusesOnStation(int id);

    void writeToFile(const String& filename, const String& output);

    std::vector<String> findIntersectionLines(const String& line);

    int getNumberOfDepartures(const String& line);

    // returns time until bus stops at station
    int getTimeDelay(Time& current, Bus& bus, int stationId);

    int findSmallestValue(std::vector<int>& vec);

    // check if bus stops at next station in path after current
    std::vector<String> checkNextStation(std::vector<String>& bussesId, int currentStation, int nextStation);

};


#endif //BUS_SYSTEM_H
