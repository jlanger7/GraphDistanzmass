#ifndef TEILGRAPHENZHKS_H
#define TEILGRAPHENZHKS_H
#include <vector>
#include <set>
#include "knoten.h"
#include "graph.h"
#include "teilgraphenSet.h"
using namespace std;

class teilgraphenZHKs{

    private:
    vector<vector<graph&>> teilgraphen;
    int anzahlTeilgraphen;
    vector<int> anzahlZHKsProGi;
    vector<int> maxVProGi;
    vector<int> minVProGi;



    public:
    teilgraphenZHKs();
    teilgraphenZHKs(teilgraphenSet& inputTeilgraphenSet);
    //Tiefensuche
        //besuche Nachbarn

};

#endif