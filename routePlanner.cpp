//
// Created by j on 7/30/25.
//
#include <set>
#include <string>
#include <fstream>
#include "routePlanner.h"
#include "GraphAdjList.h"
using namespace bridges;
using namespace std;

routePlanner::routePlanner(datastructure::GraphAdjList<int, dataset::OSMVertex, double>* _graph) {
    graph = _graph;
}

// Min-heap implementation of Dijkstra's algorithm
// this implementation is based on https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/GraphShortest.html
void routePlanner::dijkstra(int source) {
    set<int> visited;

    // create a min heap for edges
    priority_queue<pair<int, double>, vector<pair<int, double>>,
      function<bool(const pair<int, double>&,const pair<int, double>&)>> pq(
        [](const pair<int, double>& a, const pair<int, double>&b) {
      return a.second > b.second;
    });
    // initialize the queue with one vertex
    pq.push(make_pair(source, 0.0));

    // initialize distances to infinity
    for (auto v : *graph->getVertices()) {
        distance[v.first] = std::numeric_limits<double>::max();
        parent[v.first] = -1;
    }

    distance[source] = 0;

    unsigned long maxVertices = graph->getAdjacencyList().size();

    for (int i = 0; i < maxVertices; i++) {
        if (pq.empty()){return;} // all remaining vertices are unreachable
        pair<int, double> p = pq.top();
        pq.pop();
        int nearestVertex = p.first;
        while (visited.find(nearestVertex) != visited.end()) {
            if (pq.empty()){return;}
            p = pq.top();
            pq.pop();
            nearestVertex = p.first;
        }
        visited.insert(nearestVertex);
        if (distance[nearestVertex] == std::numeric_limits<double>::max()) {return;}
        auto listPtr = graph->getAdjacencyList(nearestVertex);
        while (listPtr != nullptr) {
            // update distances to neighbors of nearestVertex
            int adjacentVertex = listPtr->getValue().to();
            double newDistance = distance[nearestVertex] + graph->getEdgeData(nearestVertex, adjacentVertex);
            if (distance[adjacentVertex] > newDistance) {
                distance[adjacentVertex]  = newDistance;
                parent[adjacentVertex] = nearestVertex;
                pq.push(make_pair(adjacentVertex, distance[adjacentVertex]));
            }
            listPtr = listPtr->getNext();
        }
    }
}

// A* algorithm
void aStar(int dest) {

}

// generate the list of lat, long pairs needed to plot the route
void routePlanner::plotRoute(const int dest) {
    vector<pair<double, double>> path;
    int vertex = dest;
    while (vertex != -1) {
        auto vertexPtr = graph->getVertex(vertex);
        path.push_back(make_pair(vertexPtr->getValue().getLatitude(), vertexPtr->getValue().getLongitude()));
        vertex = parent[vertex];
    }
    generateViz(path);
}

void routePlanner::generateViz(vector<pair<double, double>>& path) {
    string fileName = "viz.html";
    ofstream file(fileName);
    string html1 = "<html><head><link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.css\" integrity=\"sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=\" crossorigin=\"\"/><script src=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.js\" integrity=\"sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=\" crossorigin=\"\"></script><title>Route Finding Demo with OpenStreetMap</title></head><body><div id=\"map\" style=\"height: 400px\"></div><script type=\"text/javascript\">var map = L.map('map').setView([0, 0], 13);var latlngs = [";
    string html2 = "];var polyline = L.polyline(latlngs, {color: 'red'}).addTo(map);map.fitBounds(polyline.getBounds());L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {maxZoom: 19, attribution: '&copy; <a href=\"http://www.openstreetmap.org/copyright\">OpenStreetMap</a>'}).addTo(map);</script></body></html>";
    file << html1 << endl;
    bool firstItem = true;
    for (auto pr: path) {
        if (!firstItem) {file << ", ";}
        firstItem = false;
        file << "[" << pr.first << ", " << pr.second << "]";
    }
    file << html2 << endl;
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
