#ifndef BUS_PARSER_H
#define BUS_PARSER_H

#include "Bus.h"
#include "Exceptions.h"
#include "Time.h"
#include "Station.h"
#include <fstream>
#include <string>
#include <vector>

typedef std::string String;

class Loader {
public:
    // makes Time obj from string (hh:mm)
    static Time loadTime(const String& time);
    // load stations from input file
    static std::vector<Station> loadStations(const String& filePath);
    // load buses
    static std::vector<Bus> loadBuses(const String& filepath);
    // load station from string, used in loadStations
    static Station loadStation(const String& line);
    // loads integer
    static int loadInt(const String& line, int& pos);
    // loads strung
    static String loadString(const String& line, int& pos);
    // loads bus
    static Bus loadBus(const String& line);
    // loads id
    static String getId(const String& line, int& pos);
    // loads time
    static String getTimeString(const String& line, int& pos);
    // loads station ids
    static std::vector<int> loadStationIds(const String& line, int& pos);
};

#endif //BUS_PARSER_H
