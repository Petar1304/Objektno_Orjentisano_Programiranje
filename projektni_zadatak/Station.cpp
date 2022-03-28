#include "Station.h"

Station::Station(int id, String name) : id(id), name(name) {}

int Station::getId() {
    return this->id;
}

String Station::getName() {
    return this->name;
}

void Station::setId(int id) {
    this->id = id;
}

void Station::setName(String name) {
    this->name = name;
}

bool operator<(const Station& station1, const Station& station2) {
    return (station1.name < station2.name);
}