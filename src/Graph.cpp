#include "knoten.h"
#include "graph.h"
#include <vector>
#include <iostream>
using namespace std;

graph::graph(vector<knoten*>& knotenMengeIn){

    knotenMenge = knotenMengeIn;
};

void graph::addKnoten(knoten* knotenIn){

    knoten.push_back(knotenIn);
};