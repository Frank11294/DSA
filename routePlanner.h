//
// Created by j on 7/30/25.
//

#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H
#include <unordered_map>

#include "GraphAdjList.h"
#include "data_src/OSMVertex.h"


class routePlanner {
    struct namedLocation {
        namedLocation(string name, double latitude, double longitude);

        string name;
        double latitude;
        double longitude;
    };

public:
    explicit routePlanner(bridges::datastructure::GraphAdjList<int, bridges::dataset::OSMVertex, double> *graph);

    int vertexFromLatLong(double lat, double lon);

    void setSrc(int source);

    void setDest(int dest);

    void dijkstra();

    void aStar();

    void plotRoute();

    vector<namedLocation> locations;

private:
    static void generateViz(vector<pair<double, double> > &path);

    bridges::datastructure::GraphAdjList<int, bridges::dataset::OSMVertex, double> *graph;
    std::unordered_map<int, double> distance;
    std::unordered_map<int, int> parent;
    int source = 0;
    int dest = 0;
};


#endif //ROUTEPLANNER_H
