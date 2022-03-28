#include "Graph.h"


// Constructor
Graph::Graph(std::vector<Station> stations, std::vector<Bus> buses) {
    createGraph(stations, buses);
}

// creating graph
void Graph::createGraph(std::vector<Station> stations, std::vector<Bus> buses) {

    for (auto& bus : buses) {
        std::vector<int> ids = bus.getStations();

        for (int i = 1; i < ids.size(); i++) {
            this->graph[ids[i-1]].insert(ids[i]);
            this->graph[ids[i]].insert(ids[i-1]);
        }
    }
}

// Calculate number of edges for the shortest path between start and end (breadth first algorithm)
std::vector<int> Graph::shortestPath(int start, int end) {

    std::vector<int> route;

    std::set<int> visited = { start };

    // map for reconstructing path
    std::unordered_map<int, int> prev;

    // gueue of [node, distance]
    std::queue<std::vector<int>> queue;
    queue.push({start, 0});

    while (!queue.empty()) {
        std::vector<int> node = queue.front();
        queue.pop();

        int id = node[0];
        int distance = node[1];

        if (id == end) {

            return reconstructPath(prev, start, end);
        }

        for (auto neighbor : this->graph[id]) {
            // check if node is already visited
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push({neighbor, distance + 1});
                prev[neighbor] = node[0];
            }
        }
    }
}


std::vector<int> Graph::reconstructPath(std::unordered_map<int, int> prev, int start, int end) {
    std::vector<int> path;
    path.push_back(end);
    int curr = end;
    while(curr != start) {
        path.push_back(prev[curr]);
        curr = prev[curr];
    }
    std::reverse(path.begin(), path.end());
    return path;
}


// Printing Graph
void Graph::printGraph() {

    for (auto& [key, value] : this->graph) {
        std::cout << key << ": ";
        for (auto el : value) {
            std::cout << el << ' ';
        }
        std::cout << std::endl;
    }
}