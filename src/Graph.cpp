#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
using namespace std;

graph::graph(){

};

graph::graph(vector<knoten*>& knotenMengeIn){

    knotenMenge = knotenMengeIn;
};

void graph::addKnoten(knoten* knotenIn){

    knotenMenge.push_back(knotenIn);
};