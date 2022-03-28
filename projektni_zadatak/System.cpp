#include "System.h"


// Constructor
System::System(const String& stations_file, const String& buses_file) {
    load(stations_file, buses_file);
}


// load Stations and Buses from input files
void System::load(const String& stations_file, const String& buses_file) {
    this->stations = Loader::loadStations(stations_file);
    this->buses = Loader::loadBuses(buses_file);
}

// prints to a file station info
void System::getStationInfo(int id) {
    String output;
    Station station = findStationById(id);
    output.append(std::to_string(station.getId()) + ' ' + station.getName() + " [");

    std::vector<String> station_buses;
    station_buses = getBusesOnStation(id);
    std::sort(station_buses.begin(), station_buses.end());

    for (const auto& bus : station_buses) {
        output.append(bus + ' ');
    }
    // removing last space
    output.pop_back();
    output.push_back(']');

    String filename = "stajaliste_"+ std::to_string(id) + ".txt";

    writeToFile(filename, output);
}


Station System::findStationById(int id) {
    for (auto& station : this->stations) {
        if (station.getId() == id) return station;
    }
}


std::vector<String> System::getBusesOnStation(int id) {
    std::vector<String> station_buses;
    for (auto& bus : this->buses) {
        for (auto& station : bus.getStations()) {
            if (station == id) {
                station_buses.push_back(bus.getLineId());
            }
        }
    }
    return station_buses;
}


// writes string output to a file
void System::writeToFile(const String& filename, const String& output) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << output;
    }
    file.close();
}


// prints bus info to a file
void System::getBusInfo(const String& line) {
    String output;
    String filename = "linija_"+ line + ".txt";

    Bus bus = findBusById(line);

    output.append(line + ' ');

    Station firstStation = findStationById(bus.getStations()[0]);
    int lastStationNum = bus.getStations().size() - 1;
    Station lastStation = findStationById(bus.getStations()[lastStationNum]);

    output.append(firstStation.getName() + "->" + lastStation.getName() + '\n');

    for (auto station : bus.getStations()) {
        output.append(std::to_string(station) + ' ');
        String stationName = findStationById(station).getName();
        output.append(stationName + '\n');
    }

    writeToFile(filename, output);
}


Bus System::findBusById(const String& line) {
    for (auto& bus : this->buses) {
        if (bus.getLineId() == line) return bus;
    }
    // else raise error "There is no bus with lineId..."
}


// writes statistics about a line to a file
void System::statistics(const String& line) {
    String output;
    String filename = "statistika_"+ line + ".txt";

    output.append(line + '\n');

    std::vector<String> intersections = findIntersectionLines(line);
    std::sort(intersections.begin(), intersections.end());

    for (auto& i : intersections) {
        output.append(i + ' ');
    }

    int departuresNum = getNumberOfDepartures(line);

    output.append('\n' + std::to_string(departuresNum));

    writeToFile(filename, output);
}


// returns vector with lines that intersect
std::vector<String> System::findIntersectionLines(const String& line) {
    Bus bus = findBusById(line);

    std::vector<String> intersections;

    bool skip = false;
    for (auto& other_bus : this->buses) {
        if (bus.getLineId() != other_bus.getLineId()) {
            for (auto& station : bus.getStations()) {
                for (auto& other_station : other_bus.getStations()) {
                    if (other_station == station) {
                        intersections.push_back(other_bus.getLineId());
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    skip = false;
                    break;
                }
            }
        }
    }
    return intersections;
}


// returns number of departures of a bus
int System::getNumberOfDepartures(const String& line) {
    Bus bus = findBusById(line);

    Time first = bus.getFirstDeparture();
    Time last = bus.getLastDeparture();
    int interval = bus.getInterval();

    int num = ((last.getHours() * 60 + last.getMinutes()) - (first.getHours() * 60 + first.getMinutes())) / interval + 1;
    return num;
}


// TRANSPORTATION SIMULATION
// find the fastest route
void System::getRoute(int start, int end, Time time) {
    String output;
    String filename = "putanja_" + std::to_string(start) + "_" + std::to_string(end) + ".txt";

    Graph graph(this->stations, this->buses);
    std::vector<int> path = graph.shortestPath(start, end);

    String lastBus = "";

    for (int i = 0; i < path.size(); i++) {
        std::vector<String> busesOnStation = getBusesOnStation(path[i]);

        if (path[i] != end) {
            // check if bus goes to next station
            busesOnStation = checkNextStation(busesOnStation, path[i], path[i + 1]);

            std::vector<int> times;
            for (auto &busId: busesOnStation) {

                Bus bus = findBusById(busId);

                int t = getTimeDelay(time, bus, path[i]);
                times.push_back(t);
            }
            int index = findSmallestValue(times);
            time.addMinutes(times[index]);
            time.addMinutes(3);

            String currentBus = busesOnStation[index];


            if (lastBus == "") {
                lastBus = currentBus;
                output.append("->" + currentBus + "\n");
                output.append(std::to_string(path[i]));
            } else if (currentBus != lastBus) {
                output.append(" " + std::to_string(path[i]));
                output.append("\n->" + currentBus + "\n");
                output.append(std::to_string(path[i]));
                lastBus = currentBus;
            } else {
                output.append(" " + std::to_string(path[i]));
            }
        }
        else {
            output.append(" " + std::to_string(path[i]));
        }
    }

    std::cout << output << std::endl;

    writeToFile(filename, output);
}


// Calculates time until bus come to station
int System::getTimeDelay(Time& current, Bus& bus, int stationId) {
    Time first = bus.getFirstDeparture();
    Time last = bus.getLastDeparture();
    int dt = bus.getInterval();

    int delay = 0;

    for (auto& station : bus.getStations()) {
        if (station == stationId) {
            break;
        }
        delay -= 3;
    }

    delay += ((current.getHours() * 60 + current.getMinutes()) - (first.getHours() * 60 + first.getMinutes())) % dt;

    return abs(delay);
}


int System::findSmallestValue(std::vector<int>& vec) {
    int index = 0;
    int smallest = vec[0];
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < smallest) {
            smallest = vec[i];
            index = i;
        }
    }
    return index;
}


// check if bus goes from current to next station
std::vector<String> System::checkNextStation(std::vector<String>& busesId, int currentStation, int nextStation) {
    std::vector<String> newBuses;

    for (auto& bus : busesId) {

        Bus curr = findBusById(bus);
        std::vector<int> stations = curr.getStations();

        for (int i = 0; i < stations.size(); i++) {
            if(stations[i] == currentStation) {
                if ((i != 0) || i != stations.size()) {
                    if ((stations[i+1] == nextStation) || (stations[i-1] == nextStation)) {
                        newBuses.push_back(bus);
                    }
                } else {
                    newBuses.push_back(bus);
                }
            }
        }
    }
    return newBuses;
}