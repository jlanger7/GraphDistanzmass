#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <set>
#include "knoten.h"
using namespace std;

class graph{

    private:
    vector<knoten*> knotenMenge;
    int anzahlZhk;
    int maxVZhk;
    int gesVt;
    vector<vector<knoten*>> zhkKnotenMengen;


    public:
    graph();
    graph(vector<knoten*>& knotenMengeIn);
    void addKnoten(knoten* knotenIn);
    void modifizierteTiefensucheNeu();

    vector<knoten*>& getKnotenMenge(){
        return knotenMenge;
    };
    int getAnzahlZhk(){
        return anzahlZhk;
    };
    int getMaxVZhk(){
        return maxVZhk;
    };
    int getGesVt(){
        return gesVt;
    };
};

#endif