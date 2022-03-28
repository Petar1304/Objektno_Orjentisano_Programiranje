#ifndef BUS_GRAPH_H
#define BUS_GRAPH_H

#include "Bus.h"
#include "Exceptions.h"
#include "Station.h"
#include "Time.h"
#include "Loader.h"
#include <algorithm>
#include <set>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

typedef std::string String;

class Graph {
public:
    Graph(std::vector<Station> stations, std::vector<Bus> buses);
    ~Graph() = default;

    // makes graph from buses and stations
    void createGraph(std::vector<Station> stations, std::vector<Bus> buses);
    // finds the shortest path between start and end
    std::vector<int> shortestPath(int start, int end);
    // reconstructs whole path
    std::vector<int> reconstructPath(std::unordered_map<int, int> prev, int start, int end);

    void printGraph();

private:
    std::unordered_map<int, std::set<int>> graph;

};

#endif //BUS_GRAPH_H
