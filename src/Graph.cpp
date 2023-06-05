#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
#include "txtFileInterface.h"
using namespace std;

graph::graph(){

};

graph::graph(vector<knoten*>& knotenMengeIn){

    knotenMenge = knotenMengeIn;
};
void graph::modifizierteTiefensuche(){

    int aktNrZhks = 0, tmpMaxV = 0, tmpMinV = 0;
    for(int v = 0; v < size(knotenMenge); v++){
        
        knoten& aktKnoten = (*knotenMenge[v]);
        if(aktKnoten.getZhkNr() == -1){

            aktNrZhks += 1;
            aktKnoten.setZhkNr(aktNrZhks);
            int tmpNrV = 1;
            cout << "Parameter vor Aufruf druchsucheNachbarn. aktNrZhks: " + to_string(aktNrZhks) + " tmpNrV: " + to_string(tmpNrV) << endl;
            durchsucheNachbarn(aktKnoten, aktKnoten.getAdjazenzListe(), aktNrZhks, tmpNrV);

            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            if(tmpMinV == 0 || tmpNrV < tmpMinV){
                tmpMinV = tmpNrV;
            }    
        }
    }
    anzahlZhk = aktNrZhks;
    maxVZhk = tmpMaxV;
    minVZhk = tmpMinV;
};

void graph::durchsucheNachbarn(knoten& v, vector<knoten*>& adjazenzListe, int &aktNrZhks, int &tmpNrV){
    
    cout << "Id akt Knoten durchsuchen: " + to_string(v.getId()) << endl;
    cout << "Parameter im Aufruf druchsucheNachbarn. aktNrZhks: " + to_string(aktNrZhks) + " tmpNrV: " + to_string(tmpNrV) << endl;
    cout << "Laenge Adjazenz1: " + to_string(size(adjazenzListe)) << endl;
    cout << "Laenge Adjazenz1: " + to_string(size(v.getAdjazenzListe())) << endl;
    for(int u = 0; u < size(adjazenzListe); u++){

        if((*adjazenzListe[u]).getZhkNr() == -1){
            
            (*adjazenzListe[u]).setZhkNr(aktNrZhks);
            tmpNrV += 1;
            durchsucheNachbarn((*adjazenzListe[u]), (*adjazenzListe[u]).getAdjazenzListe(), aktNrZhks, tmpNrV);
        }
    }
};

void graph::addKnoten(knoten* knotenIn){

    knotenMenge.push_back(knotenIn);
};

void graph::berechneKanten(txtFileInterface txt){

    int nrTotalVertices = size(knotenMenge);

    for(int v = 0; v < nrTotalVertices-1; v++){

        if(v%250000 == 0){
            std::cout << "addEdges, betrachte Knoten nr: " + to_string(v) << endl;
        }

        int gridId = (*knotenMenge[v]).getId();

        int* coordinates = txt.calculateCoordinates(gridId);
        int timeStep = *(coordinates + 0);
        int x = *(coordinates + 1);
        int y = *(coordinates + 2);

        int potNeighbor = v + 1;
        int gridIdPotNeighbor = (*knotenMenge[potNeighbor]).getId();
       
        while((gridIdNeighbor - gridId) <= ((txt.getDimX()*txt.getDimY())+txt.getDimX()) && potNeighbor < nrTotalVertices){

            if(gridIdNeighbor == txt.calculateGridId(timeStep,x,y+1) || 
                gridIdNeighbor == txt.calculateGridId(timeStep,x+1,y) ||
                    gridIdNeighbor == txt.calculateGridId(timeStep+1,x,y)){

                        (*knotenMenge[v]).addNachbarn(knotenMenge[potNeighbor]);
                        (*knotenMenge[potNeighbor]).addNachbarn(knotenMenge[v]);
                }
            potNeighbor += 1; 
            gridIdNeighbor = (*knotenMenge[potNeighbor]).getId();
        }
        delete[] coordinates;
    }
};