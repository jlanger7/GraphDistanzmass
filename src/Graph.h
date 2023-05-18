#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <set>
#include "knoten.h"
using namespace std;

class graph{

    private:
    vector<knoten> knotenMenge;


    public:
    graph();
    graph(vector<knoten>& knotenMengeIn);

    void addKnoten(knoten& knotenIn);
    vector<knoten>& getKnotenMenge(){
        return knotenMenge;
    };


};

#endif