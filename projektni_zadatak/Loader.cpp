#include "Loader.h"


Time Loader::loadTime(const String& time) {
    String hh;
    String mm;
    int i = 0;
    while(time[i] != ':') {
        hh.push_back(time[i]);
        i++;
    }
    i++;
    while(i < 5) {
        mm.push_back(time[i]);
        i++;
    }
    return Time(stoi(hh), stoi(mm));
}


std::vector<Station> Loader::loadStations(const String& filePath) {
    std::vector<Station> stations;
    String line;
    std::ifstream input(filePath);

    try {
        if (input.is_open()) {
            // load line by line
            while (std::getline(input, line)) {
                stations.push_back(loadStation(line));
            }
        }
        else {
            // trow exception for closed file
            throw "ERROR >> file closed";
        }
    }
    catch (Error& e) {
        std::cout << e.what() << std::endl;
    }

    return stations;
}


// loadStation from line
Station Loader::loadStation(const String& line) {
    int id;
    String name;
    int pos = 0;

    id = loadInt(line, pos);
    pos++;
    name = loadString(line, pos);

    return Station(id, name);
}


int Loader::loadInt(const String& line, int& pos) {
    String num;
    while(line[pos] != ' ' && line[pos] != '\n' && line[pos] != '\0' && line[pos] != ']' && line[pos] != '\r') {
        num.push_back(line[pos]);
        pos++;
    }
    return stoi(num);
}


String Loader::loadString(const String& line, int& pos) {
    String name;
    while(line[pos] != '\n' && line[pos] != '\0' && line[pos] != '\r') {
        name.push_back(line[pos]);
        pos++;
    }
    return name;
}


// id [hh:mm-hh:mm#interval] stations
// load Buses from input file int vector
std::vector<Bus> Loader::loadBuses(const String& filepath) {
    std::vector<Bus> buses;

    std::ifstream input(filepath);
    std::string line;

    if (input.is_open()) {
        while(std::getline(input, line)) {
            buses.push_back(loadBus(line));
        }
    }
    return buses;
}


Bus Loader::loadBus(const String& line) {
    int pos = 0;
    String lineId = getId(line, pos);
    pos += 2; // skipping '['
    Time firstDeparture = loadTime(getTimeString(line, pos));
    pos++; // skipping '-'
    Time lastDeparture = loadTime(getTimeString(line, pos));
    pos++; // skipping '#'
    int interval = loadInt(line, pos);
    pos += 2; // skip last space before ids
    std::vector<int> stationsIds = loadStationIds(line, pos);

    return Bus(lineId, stationsIds, firstDeparture, lastDeparture, interval);
}



String Loader::getId(const String& line, int& pos) {
    std::string str;
    while (line[pos] != ' ') {
        str.push_back(line[pos]);
        pos++;
    }
    return str;
}


// Slice string to format hh:mm
String Loader::getTimeString(const String& line, int& pos) {
    String time;
    for (int i = 0; i < 5; i++) {
        time.push_back(line[pos]);
        pos++;
    }
    return time;
}


std::vector<int> Loader::loadStationIds(const String& line, int& pos) {
    std::vector<int> ids;
    while(pos < line.length()) {
        ids.push_back(loadInt(line, pos)); // skips next space
        pos++;
    }
    return ids;
}

