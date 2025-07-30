//
// Created by j on 7/30/25.
//

#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H
#include <unordered_map>

#include "GraphAdjList.h"
#include "data_src/OSMVertex.h"


class routePlanner {
public:
  explicit routePlanner(bridges::datastructure::GraphAdjList<int, bridges::dataset::OSMVertex, double>* graph);
  void dijkstra(int source);
  void aStar(int dest);
  void plotRoute(int dest);

private:
  bridges::datastructure::GraphAdjList<int, bridges::dataset::OSMVertex, double>* graph;
  std::unordered_map<int, double> distance;
  std::unordered_map<int, int> parent;
};


#endif //ROUTEPLANNER_H
