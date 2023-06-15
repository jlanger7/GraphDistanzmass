#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
using namespace std;

graph::graph(){

};

graph::graph(vector<knoten*>& knotenMengeIn){

    cout << "       Konstruktor Graph Start" << endl;
    knotenMenge = knotenMengeIn;
    cout << "       Konstruktor Graph Ende" << endl;
};
void graph::modifizierteTiefensuche(){

    int aktNrZhks = 0, tmpMaxV = 0, tmpMeanV = 0;
    for(int v = 0; v < size(knotenMenge); v++){
        
        knoten& aktKnoten = (*knotenMenge[v]);
        if(aktKnoten.getZhkNr() == -1){

            aktNrZhks += 1;
            aktKnoten.setZhkNr(aktNrZhks);
            int tmpNrV = 1;
            //cout << "Parameter vor Aufruf druchsucheNachbarn. aktNrZhks: " + to_string(aktNrZhks) + " tmpNrV: " + to_string(tmpNrV) << endl;
            durchsucheNachbarn(aktKnoten, aktKnoten.getAdjazenzListe(), aktNrZhks, tmpNrV);

            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            tmpMeanV += tmpNrV;
        }
    }
    anzahlZhk = aktNrZhks;
    maxVZhk = tmpMaxV;
    meanVZhk = tmpMeanV/anzahlZhk;
};

void graph::durchsucheNachbarn(knoten& v, vector<knoten*>& adjazenzListe, int &aktNrZhks, int &tmpNrV){
    
    /*cout << "Id akt Knoten durchsuchen: " + to_string(v.getId()) << endl;
    cout << "Parameter im Aufruf druchsucheNachbarn. aktNrZhks: " + to_string(aktNrZhks) + " tmpNrV: " + to_string(tmpNrV) << endl;
    cout << "Laenge Adjazenz1: " + to_string(size(adjazenzListe)) << endl;
    cout << "Laenge Adjazenz1: " + to_string(size(v.getAdjazenzListe())) << endl;*/
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

