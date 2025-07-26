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

//function used to return the relative distance between doubles.
//useful to compare for equality
double RelDif(double a, double b) {
  double c = std::abs(a);
  double d = std::abs(b);

  d = (std::max)(c, d);

  return d == 0.0 ? 0.0 : std::abs(a - b) / d;
}


//used to get the coordinate of the (.25,.25) of the map
void getQuarter(const OSMData& osm_data, double& lat, double& lon) {
  double latr[2];
  double lonr[2];
  osm_data.getLatLongRange(latr, lonr);

  lat = latr[0] + (latr[1] - latr[0]) / 4.;
  lon = lonr[0] + (lonr[1] - lonr[0]) / 4.;
}

//used to get the coordinate of the center of the map
void getCenter(const OSMData& osm_data, double& lat, double& lon) {
  double latr[2];
  double lonr[2];
  osm_data.getLatLongRange(latr, lonr);

  lat = (latr[0] + latr[1]) / 2.;
  lon = (lonr[0] + lonr[1]) / 2.;
}

//actual shortestPath implementation
// this implementation is based partially on https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/GraphShortest.html
void shortestPath (const GraphAdjList<int, OSMVertex, double>& gr,
               int source,
               std::unordered_map<int, double>& distance,
               std::unordered_map<int, int>& parent) {
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

//style all vertices based on their distance to the root of the shortest path.
//style edges based on whether they sit on a shortest path or not
void styleDistance(GraphAdjList<int, OSMVertex, double> graph,
                   const std::unordered_map<int, double>& distance) {
  double maxd = 0.;

  //find max distance (Beware of unreachable vertices with a distance of INFINITY)


  //color vertices based on distances

  //optional: style edges if they are shortest path edges. (Beware of back edges)
}

//style graph based on whether vertices and edges sit on the shortest path between dest and source. (Note that source is not given since all parent pointer chase go there)
void styleParent(GraphAdjList<int, OSMVertex, double> graph,
                 const std::unordered_map<int, double>& distance,
                 const std::unordered_map<int, int>& parent,
                 int dest
                ) {
  //TODO

  //set all edges to transparent

  //set all vertices to transparent


  //for each edge on the SP from source to dest
}

//change the style of the root of the shortest path
void styleRoot(GraphAdjList<int, OSMVertex, double>& graph,
               int root) {
  //TODO
}

int main(int argc, char **argv) {
  //SFML window
  /*float width = 1600, height = 1000;
  sf::RenderWindow window(sf::VideoMode(width, height), "USA Tour Planner", sf::Style::Close);

  gui ui(window, width, height);
  ui.run();
*/
  // Using the BRIDGES API to get data
  Bridges bridges (1, "BenN5334", "574789216298");
  bridges.setTitle("Graph : OpenStreetMap Example");

  //Getting Data
  //int closest;
  //double latc, lonc;
  //int dest;

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
  graph.forceLargeVisualization(true);
  bridges.setDataStructure(&graph);
  //bridges.visualize();

  //TODO Uncomment for part 2
  // //Getting source vertex (Using center of the map)
  // getCenter(osm_data, latc, lonc);
  // closest = getClosestVertex(graph, latc, lonc);
  // //Getting destination vertex
  // getQuarter(osm_data, latc, lonc);
  // dest = getClosestVertex(graph, latc, lonc);
  // styleRoot(graph, closest);
  // bridges.setDataStructure(&graph);
  // bridges.visualize();


  //TODO Uncomment for part 3.
  // //Running shortest path
/*
  cout << "Adjacency list:" << endl;
  for (auto e : graph.getAdjacencyList()) {
    cout << e.first << " ";
    auto listPtr = graph.getAdjacencyList(e.first);
    while (listPtr) {
      cout << listPtr->getValue().to() << " (" << graph.getEdgeData(e.first, listPtr->getValue().to()) << ") ";
      listPtr = listPtr->getNext();
    }
    cout << endl;
  }*/

  int source = graph.getVertices()->begin()->first;
  // cout << "source vertex: " << source << endl;
  std::unordered_map<int, double> distance;
  std::unordered_map<int, int> parent;
  shortestPath(graph, source, distance, parent);
  /*cout << "Vertex Distance Parent" << endl;
  for (auto v: *graph.getVertices()) {
    cout << v.first << " " << distance[v.first] << " " << parent[v.first] << endl;
  }*/
  // //Styling based on distance
  // styleDistance(graph, distance);
  // bridges.visualize();

  //TODO Uncomment for part 4
  // //styling based on source-destination path
  // styleParent(graph, distance, parent, dest);
  // bridges.visualize();

    return 0;
}
