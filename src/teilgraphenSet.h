#ifndef TEILGRAPHENSET_H
#define TEILGRAPHENSET_H
#include <vector>
#include <set>
#include "knoten.h"
#include "graph.h"
using namespace std;

class teilgraphenSet{

    private:
    vector<graph&> teilgraphen;
    graph& gesamtGraph;
    int anzahlTeilgraphen;


    public:
    teilgraphenSet();
    teilgraphenSet(graph& inputGraph);
    void erstelleTeilgraphenSet();
    /
    int getAnzahlTeilgraphen(){
        return anzahlTeilgraphen;
    };

};

#endif