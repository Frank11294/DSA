#include "Bridges.h"
#include "DataSource.h"
#include "GraphAdjList.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

#include "gui.h"
#include "data_src/OSMData.h"
#include "data_src/OSMVertex.h"
#include "data_src/OSMEdge.h"
using namespace std;
using namespace bridges;

/******************************
 * This file is based on a template provided by BRIDGES.
 * Refer to https://bridgesuncc.github.io/assignments/data//9-ShortestPathOSM/README.html
 * and https://bridgesuncc.github.io/tutorials/Data_OSM.html
 ******************************/

// Min-heap implementation of Dijkstra's algorithm
// this implementation is based partially on https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/GraphShortest.html
void shortestPath (const GraphAdjList<int, OSMVertex, double>& gr,
               int source,
               std::unordered_map<int, double>& distance,
               std::unordered_map<int, int>& parent
               ) {
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
  for (auto v : *gr.getVertices()) {
    distance[v.first] = std::numeric_limits<double>::max();
    parent[v.first] = -1;
  }

  distance[source] = 0;

  unsigned long maxVertices = gr.getAdjacencyList().size();

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
    auto listPtr = gr.getAdjacencyList(nearestVertex);
    while (listPtr != nullptr) {
      // update distances to neighbors of nearestVertex
      int adjacentVertex = listPtr->getValue().to();
      double newDistance = distance[nearestVertex] + gr.getEdgeData(nearestVertex, adjacentVertex);
      if (distance[adjacentVertex] > newDistance) {
        distance[adjacentVertex]  = newDistance;
        parent[adjacentVertex] = nearestVertex;
        pq.push(make_pair(adjacentVertex, distance[adjacentVertex]));
      }
      listPtr = listPtr->getNext();
    }
  }
}

//return the vertex the closest to a particular (lat,lon)
int getClosestVertex(const GraphAdjList<int, OSMVertex, double>& graph,
                     double lat, double lon) {
  return -1;
}

// generate the list of lat, long pairs needed to plot the route
void plotRoute(GraphAdjList<int, OSMVertex, double>& graph, std::unordered_map<int, int>& parent, const int dest) {
  int vertex = dest;
  bool firstItem = true;
  cout << "[";
  while (vertex != -1) {
    auto vertexPtr = graph.getVertex(vertex);
    if (!firstItem) {
      cout << ", ";
    }
    firstItem = false;
    cout << "[" << vertexPtr->getValue().getLatitude() << ", " << vertexPtr->getValue().getLongitude()  << "]";
    vertex = parent[vertex];
  }
  cout << "]" << endl;
}

int main(int argc, char **argv) {
  //SFML window
  float width = 1600, height = 1000;
  sf::RenderWindow window(sf::VideoMode(width, height), "USA Tour Planner", sf::Style::Close);

  gui ui(window, width, height);
  ui.run();

  // Using the BRIDGES API to get data
  Bridges bridges (1, "BenN5334", "574789216298");
  bridges.setTitle("Graph : OpenStreetMap Example");

  DataSource ds (&bridges);
  OSMData osm_data = ds.getOSMData("Charlotte, North Carolina", "tertiary");
  //OSMData osm_data = ds.getOSMData(39.85, -83.14, 40.12, -82.85, "secondary"); //Columbus, OH
  //OSMData osm_data = ds.getOSMData(39.121, -77.055, 39.208, -76.805); //Baltimore, MD

  // How to get edges and vertices from data set
  vector<OSMVertex> vertices = osm_data.getVertices();
  vector<OSMEdge> edges = osm_data.getEdges();

  cout << "Data set has " << vertices.size() << " vertices and " << edges.size() << " edges" << endl;


  GraphAdjList<int, OSMVertex, double> graph;
  osm_data.getGraph (&graph);
  bridges.setDataStructure(&graph);

  // choosing arbitrary source and destination to test visualization
  // TODO: use UI to specify source and destination
  int source = graph.getVertices()->begin()->first;
  auto ptr = graph.getVertices()->begin();
  for (int i = 0; i < 1001 && i < graph.getVertices()->size(); i++) {ptr++;}
  int dest = ptr->first;

  // distance and parent vertex tables for Dijkstra's algorithm
  std::unordered_map<int, double> distance;
  std::unordered_map<int, int> parent;
  shortestPath(graph, source, distance, parent);

  plotRoute(graph, parent, dest);

  return 0;
}
