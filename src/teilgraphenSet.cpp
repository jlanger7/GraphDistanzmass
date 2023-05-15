#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
#include "teilgraphenSet.h"
using namespace std;

teilgraphenSet::teilgraphenSet(){


};

teilgraphenSet::teilgraphenSet(graph& inputGraph){

    gesamtGraph = inputGraph;
};

void teilgraphenSet::erstelleTeilgraphenSet(){

    teilgraphen = new vector<graph>;

    int aktMaxZeitattribut = 0;
    vector<knoten*>& gesamtKnotenMenge = gesamtGraph.getKnotenMenge();

    for(int v = 0; v < size(gesamtKnotenMenge); v++){
        
        cout << "V nr: " + to_string(v) << endl;
        knoten* aktKnoten = gesamtKnotenMenge[v];
        int zeitAttributAktKnoten = (*aktKnoten).getZeitAttribut();
        if(zeitAttributAktKnoten > aktMaxZeitattribut){

            int differenz = zeitAttributAktKnoten - aktMaxZeitattribut;
            cout << "differenz: " + to_string(differenz) << endl;
            aktMaxZeitattribut = zeitAttributAktKnoten;
            for(int i = 0; i < differenz; i++){

                graph teilgraph;
                (*teilgraphen).push_back(teilgraph);
            }
        }
        (*teilgraphen)[zeitAttributAktKnoten-1].addKnoten(aktKnoten);
        for(int n = 0; n < size((*aktKnoten).getAdjazenzListe()); n++){

            if(!((*(*aktKnoten).getAdjazenzListe()[n]).getZeitAttribut() == zeitAttributAktKnoten)){
                cout << "loesche: " + to_string((*(*aktKnoten).getAdjazenzListe()[n]).getId()) + " von Knoten: " + to_string((*aktKnoten).getId()) << endl;
                (*aktKnoten).getAdjazenzListe().erase((*aktKnoten).getAdjazenzListe().begin()+n);
            }
        }
    }
    cout << "aktMaxZeitattribut: " + to_string(aktMaxZeitattribut) << endl;
    anzahlTeilgraphen = aktMaxZeitattribut;
    cout << "anzahl Teilgraphen: " + to_string(anzahlTeilgraphen) << endl;
};