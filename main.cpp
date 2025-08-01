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

  // Using the BRIDGES API to get data
  Bridges bridges (1, "BenN5334", "574789216298");
  bridges.setTitle("Graph : OpenStreetMap Example");

  DataSource ds (&bridges);
  OSMData osm_data = ds.getOSMData("New York, New York", "tertiary");
  cout << "Data set has " << osm_data.getVertices().size() << " vertices and " << osm_data.getEdges().size() << " edges" << endl;

  GraphAdjList<int, OSMVertex, double> graph;
  osm_data.getGraph (&graph);
  bridges.setDataStructure(&graph);

  routePlanner rp(&graph);

  //SFML window
  float width = 1600, height = 1000;
  sf::RenderWindow window(sf::VideoMode(width, height), "Route Finding Demo", sf::Style::Close);

  gui ui(window, width, height);
  ui.setRoutePlanner(&rp);
  ui.run();

  return 0;
}
