//
// Created by j on 7/30/25.
//
#include <set>
#include <string>
#include <fstream>
#include "routePlanner.h"
#include "GraphAdjList.h"

#include <queue>
#include <functional>
#include <cmath>
#include <windows.h>
using namespace bridges;
using namespace std;

routePlanner::namedLocation::namedLocation(string name, double latitude, double longitude): name(name),
    latitude(latitude), longitude(longitude) {
}

routePlanner::routePlanner(datastructure::GraphAdjList<int, dataset::OSMVertex, double> *_graph) {
    graph = _graph;
    locations = {
        namedLocation("New York Public Library", 40.75331230968659, -73.98221048749915),
        namedLocation("Metropolitan Opera", 40.77284569648566, -73.98387601633382),
        namedLocation("Museum of Modern Art", 40.76152206165067, -73.9776001451696),
        namedLocation("9/11 Memorial", 40.71175648582668, -74.01334693167757),
        namedLocation("Madison Square Garden", 40.753794615995474, -73.99251715096007),
        namedLocation("Central Park", 40.78295679875711, -73.9655404874981),
        namedLocation("Brooklyn Bridge", 40.7063258269255, -73.99681040200315),
        namedLocation("Times Square", 40.75882937544937, -73.98569280653601),
        namedLocation("Hotel Mimosa", 40.71442725251007, -73.9937488808017),
        namedLocation("Wall Street", 40.70619760179813, -74.00888314517158)
    };
}

// finds the connected vertex nearest to a given latitude, longitude pair
int routePlanner::vertexFromLatLong(double lat, double lon) {
    double bestVertex = 0;
    double bestSquaredDist = std::numeric_limits<double>::max();
    for (auto vertexPtr = graph->getAdjacencyList().begin(); vertexPtr != graph->getAdjacencyList().end(); vertexPtr
         ++) {
        auto vertex = graph->getVertex(vertexPtr->first)->getValue();
        double latDiff = lat - vertex.getLatitude();
        double longDiff = lon - vertex.getLongitude();
        double squaredDist = latDiff * latDiff + longDiff * longDiff;
        if (squaredDist < bestSquaredDist && graph->getAdjacencyList(vertexPtr->first) != nullptr) {
            bestVertex = vertexPtr->first;
            bestSquaredDist = squaredDist;
        }
    }
    return bestVertex;
}

void routePlanner::setSrc(int src) {
    source = src;
}

void routePlanner::setDest(int dst) {
    dest = dst;
}

// Min-heap implementation of Dijkstra's algorithm
// this implementation is based on https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/GraphShortest.html
void routePlanner::dijkstra() {
    set<int> visited;
    parent.clear();
    distance.clear();

    // create a min heap for edges
    priority_queue<pair<int, double>, vector<pair<int, double> >,
        function<bool(const pair<int, double> &, const pair<int, double> &)> > pq(
        [](const pair<int, double> &a, const pair<int, double> &b) {
            return a.second > b.second;
        });
    // initialize the queue with one vertex
    pq.push(make_pair(source, 0.0));

    // initialize distances to infinity
    for (auto v: *graph->getVertices()) {
        distance[v.first] = std::numeric_limits<double>::max();
        parent[v.first] = -1;
    }

    distance[source] = 0;

    unsigned long maxVertices = graph->getAdjacencyList().size();

    for (int i = 0; i < maxVertices; i++) {
        if (pq.empty()) { return; } // all remaining vertices are unreachable
        pair<int, double> p = pq.top();
        pq.pop();
        int nearestVertex = p.first;
        while (visited.find(nearestVertex) != visited.end()) {
            if (pq.empty()) { return; }
            p = pq.top();
            pq.pop();
            nearestVertex = p.first;
        }
        visited.insert(nearestVertex);
        if (distance[nearestVertex] == std::numeric_limits<double>::max()) { return; }
        auto listPtr = graph->getAdjacencyList(nearestVertex);
        while (listPtr != nullptr) {
            // update distances to neighbors of nearestVertex
            int adjacentVertex = listPtr->getValue().to();
            double newDistance = distance[nearestVertex] + graph->getEdgeData(nearestVertex, adjacentVertex);
            if (distance[adjacentVertex] > newDistance) {
                distance[adjacentVertex] = newDistance;
                parent[adjacentVertex] = nearestVertex;
                pq.push(make_pair(adjacentVertex, distance[adjacentVertex]));
            }
            if (adjacentVertex == dest) { return; }
            listPtr = listPtr->getNext();
        }
    }
}

// A* algorithm
void routePlanner::aStar() {
    set<int> visited;
    parent.clear();
    distance.clear();

    // create a min heap for edges
    priority_queue<pair<int, double>, vector<pair<int, double> >,
        function<bool(const pair<int, double> &, const pair<int, double> &)> > pq(
        [](const pair<int, double> &a, const pair<int, double> &b) {
            return a.second > b.second;
        });
    // initialize the queue with one vertex
    pq.push(make_pair(source, 0.0));

    // initialize distances to infinity
    for (auto v: *graph->getVertices()) {
        distance[v.first] = std::numeric_limits<double>::max();
        parent[v.first] = -1;
    }

    distance[source] = 0;

    unsigned long maxVertices = graph->getAdjacencyList().size();

    for (int i = 0; i < maxVertices; i++) {
        if (pq.empty()) { return; } // all remaining vertices are unreachable
        pair<int, double> p = pq.top();
        pq.pop();
        int bestVertex = p.first;
        while (visited.find(bestVertex) != visited.end()) {
            if (pq.empty()) { return; }
            p = pq.top();
            pq.pop();
            bestVertex = p.first;
        }
        visited.insert(bestVertex);
        if (distance[bestVertex] == std::numeric_limits<double>::max()) { return; }
        auto listPtr = graph->getAdjacencyList(bestVertex);
        while (listPtr != nullptr) {
            // update distances to neighbors of bestVertex
            int adjacentVertex = listPtr->getValue().to();
            double newDistance = distance[bestVertex] + graph->getEdgeData(bestVertex, adjacentVertex);
            if (distance[adjacentVertex] > newDistance) {
                distance[adjacentVertex] = newDistance;
                parent[adjacentVertex] = bestVertex;
                double latDiff = graph->getVertex(listPtr->getValue().from())->getValue().getLatitude() - graph->
                                 getVertex(listPtr->getValue().to())->getValue().getLatitude();
                double longDiff = graph->getVertex(listPtr->getValue().from())->getValue().getLongitude() - graph->
                                  getVertex(listPtr->getValue().to())->getValue().getLongitude();
                double estTotalDistToDest = newDistance + 10000 * sqrt(latDiff * latDiff + longDiff * longDiff);
                pq.push(make_pair(adjacentVertex, estTotalDistToDest));
            }
            if (adjacentVertex == dest) { return; }
            listPtr = listPtr->getNext();
        }
    }
}

// generate the list of lat, long pairs needed to plot the route
void routePlanner::plotRoute() {
    vector<pair<double, double> > path;
    int vertex = dest;
    while (vertex != -1) {
        auto vertexPtr = graph->getVertex(vertex);
        path.push_back(make_pair(vertexPtr->getValue().getLatitude(), vertexPtr->getValue().getLongitude()));
        vertex = parent[vertex];
    }
    generateViz(path);
}

void routePlanner::generateViz(vector<pair<double, double> > &path) {
    string fileName = "viz.html";
    ofstream file(fileName);
    string html1 = R"(<html><head>
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=" crossorigin=""/>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js" integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=" crossorigin=""></script>
<title>Route Finding Demo with OpenStreetMap</title></head>
<body><div id="map" style="height: 100%"></div>
<script type="text/javascript">var map = L.map('map').setView([0, 0], 13);var latlngs = [)";
    string html2 = R"(];
var polyline = L.polyline(latlngs, {color: 'red'}).addTo(map);
map.fitBounds(polyline.getBounds());
L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {maxZoom: 19, attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'}).addTo(map);
var startPopup = L.popup().setLatLng([)";
    string html3 = "]).setContent(\"Destination\").addTo(map);\nvar destPopup = L.popup().setLatLng([";
    string html4 = "]).setContent(\"Start\").addTo(map);</script></body></html>";
    file << html1 << endl;
    bool firstItem = true;
    for (auto pr: path) {
        if (!firstItem) { file << ", "; }
        firstItem = false;
        file << "[" << pr.first << ", " << pr.second << "]";
    }
    file << html2 << endl;
    file << path[0].first << ", " << path[0].second;
    file << html3 << endl;
    file << path[path.size() - 1].first << ", " << path[path.size() - 1].second;
    file << html4 << endl;
    file.close();
#ifdef _WIN32
    ShellExecute(nullptr, "open", fileName.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    std::string command = "open " + fileName;
    system(command.c_str());
#else
    std::string command = "xdg-open " + fileName;
    system(command.c_str());
#endif
}
