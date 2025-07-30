//
// Created by j on 7/30/25.
//
#include <set>
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
    int vertex = dest;
    bool firstItem = true;
    cout << "[";
    while (vertex != -1) {
        auto vertexPtr = graph->getVertex(vertex);
        if (!firstItem) {
            cout << ", ";
        }
        firstItem = false;
        cout << "[" << vertexPtr->getValue().getLatitude() << ", " << vertexPtr->getValue().getLongitude()  << "]";
        vertex = parent[vertex];
    }
    cout << "]" << endl;
}
