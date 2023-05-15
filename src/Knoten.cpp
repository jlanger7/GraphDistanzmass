#include "knoten.h"
#include <vector>
#include <iostream>
using namespace std;

knoten::knoten(){

};

knoten::knoten(int idIn, int zeitAttributIn){

    id = idIn;
    zeitAttribut = zeitAttributIn;
};

void knoten::addNachbarn(int nachbarId){

    adjazenzListe.push_back(nachbarId);
};