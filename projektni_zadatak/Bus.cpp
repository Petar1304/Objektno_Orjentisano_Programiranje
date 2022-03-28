#include "Bus.h"

// Constructor
Bus::Bus(String lineId, std::vector<int> stationsIds, Time firstDeparture, Time lastDeparture, int interval) : lineId(lineId), stationsIds(stationsIds), firstDeparture(firstDeparture), lastDeparture(lastDeparture), interval(interval) {}

String Bus::getLineId() {
    return this->lineId;
}

std::vector<int> Bus::getStations() {
    return this->stationsIds;
}

Time Bus::getFirstDeparture() {
    return this->firstDeparture;
}

Time Bus::getLastDeparture() {
    return this->lastDeparture;
}

int Bus::getInterval() {
    return this->interval;
}

void Bus::setLineId(int lineId) {
    this->lineId = lineId;
}
void Bus::setStations(const std::vector<int>& stationsIds) {
    this->stationsIds = stationsIds;
}

void Bus::setFirstDeparture(const Time& time) {
    this->firstDeparture = time;
}

void Bus::setLastDeparture(const Time& time) {
    this->lastDeparture = time;
}

void Bus::setInterval(int interval) {
    this->interval = interval;
}