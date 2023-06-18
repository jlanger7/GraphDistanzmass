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
    int meanVZhk;
    vector<vector<knoten*>> zhkKnotenMengen;


    public:
    graph();
    graph(vector<knoten*>& knotenMengeIn);
    void addKnoten(knoten* knotenIn);
    void modifizierteTiefensucheNeu();
    void modifizierteTiefensuche();
    void durchsucheNachbarn(knoten& v, vector<knoten*>& adjazenzListe, int &aktNrZhks, int &tmpNrV);

    vector<knoten*>& getKnotenMenge(){
        return knotenMenge;
    };
    int getAnzahlZhk(){
        return anzahlZhk;
    };
    int getMaxVZhk(){
        return maxVZhk;
    };
    int getMeanVZhk(){
        return meanVZhk;
    };


};

#endif