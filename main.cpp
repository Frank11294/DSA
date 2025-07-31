#include "Bridges.h"
#include "DataSource.h"
#include "GraphAdjList.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

#include "gui.h"
#include "routePlanner.h"
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

int main(int argc, char **argv) {
  //SFML window
  float width = 1600, height = 1000;
  sf::RenderWindow window(sf::VideoMode(width, height), "Route Finding Demo", sf::Style::Close);

  gui ui(window, width, height);
  ui.run();

  // Using the BRIDGES API to get data
  Bridges bridges (1, "BenN5334", "574789216298");
  bridges.setTitle("Graph : OpenStreetMap Example");

  DataSource ds (&bridges);
  OSMData osm_data = ds.getOSMData("Charlotte, North Carolina", "tertiary");

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

  routePlanner rp(&graph);
    rp.setSrc(source);
  rp.setDest(dest);
  rp.dijkstra();
  rp.plotRoute();

  return 0;
}
