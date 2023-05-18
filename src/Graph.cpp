#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
using namespace std;

graph::graph(){

};

graph::graph(vector<knoten>& knotenMengeIn){

    knotenMenge = knotenMengeIn;
};
/*
void graph::modifizierteTiefensuche(){

    int aktNrZhks, tmpMaxV, tmpMinV = 0;
    for(int v = 0; v < size(knotenMenge); v++){
        
        knoten& aktKnoten = knotenMenge[v];
        if(aktKnoten.getZhkNr() == -1){

            aktNrZhks += 1;
            aktKnoten.setZhkNr(aktNrZhks);
            int tmpNrV = 1;
            durchsucheNachbarn(aktKnoten, aktKnoten.getAdjazenzListe(), aktNrZhks, tmpNrV);

            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            if(tmpMinV == 0 || tmpNrV < tmpMinV){
                tmpMinV = tmpNrV;
            }    
        }
    }
    maxVZhk = tmpMaxV;
    minVZhk = tmpMinV;
};

void graph::durchsucheNachbarn(knoten& v, vector<knoten>& adjazenzListe, int &aktNrZhks, int &tmpNrV){
    
    cout << "Id akt Knoten durchsuchen: " + to_string(v.getId()) << endl;
    for(int u = 0; u < size(adjazenzListe); u++){

        if(adjazenzListe[u].getZhkNr() == -1){
            
            adjazenzListe[u].setZhkNr(aktNrZhks);
            tmpNrV += 1;
            durchsucheNachbarn(adjazenzListe[u], adjazenzListe[u].getAdjazenzListe(), aktNrZhks, tmpNrV);
        }
    }
};
*/
void graph::addKnoten(knoten& knotenIn){

    knotenMenge.push_back(knotenIn);
};