#ifndef BUS_STATION_H
#define BUS_STATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef std::string String;

class Station {
public:
    Station(int id, String name);
    ~Station() = default;

    int getId();
    String getName();
    void setId(int id);
    void setName(String name);

    // overriding operator to use std::sort later
    friend bool operator < (const Station& station1, const Station& station2);

private:
    int id;
    String name;

};


#endif //BUS_STATION_H
