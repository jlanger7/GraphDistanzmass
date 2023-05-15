#include <vector>
#include <iostream>
#include "knoten.h"
using namespace std;

knoten::knoten(){

};

knoten::knoten(int idIn, int zeitAttributIn){

    id = idIn;
    zeitAttribut = zeitAttributIn;
};

void knoten::addNachbarn(knoten* nachbar){

    adjazenzListe.push_back(nachbar);
};