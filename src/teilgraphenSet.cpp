#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
#include "teilgraphenSet.h"
using namespace std;

teilgraphenSet::teilgraphenSet(){


};

teilgraphenSet::teilgraphenSet(graph* inputGraph){

    gesamtGraph = inputGraph;
    erstelleTeilgraphenSet();
};

void teilgraphenSet::erstelleTeilgraphenSet(){

    vector<knoten*> gesamtKnotenMenge = (*gesamtGraph).getKnotenMenge();
    int aktMaxZeitattribut = 0;

    for(int v = 0; v < size(gesamtKnotenMenge); v++){
        
        knoten* aktKnoten = gesamtKnotenMenge[v];
        int zeitAttributAktKnoten = (*aktKnoten).getZeitAttribut();
        if(zeitAttributAktKnoten > aktMaxZeitattribut){

            int differenz = zeitAttributAktKnoten - aktMaxZeitattribut;
            //cout << "differenz: " + to_string(differenz) << endl;
            aktMaxZeitattribut = zeitAttributAktKnoten;
            for(int i = 0; i < differenz; i++){

                graph teilgraph;
                teilgraphen.push_back(teilgraph);
                /*
                cout << "Teilgraph fuer Te: " + to_string(zeitAttributAktKnoten) << endl;
                */
            }
        }
        teilgraphen[zeitAttributAktKnoten-1].addKnoten(aktKnoten);
        for(int n = 0; n < size((*aktKnoten).getAdjazenzListe()); n++){

            if(!((*(*aktKnoten).getAdjazenzListe()[n]).getZeitAttribut() == zeitAttributAktKnoten)){
                //cout << "loesche: " + to_string((*(*aktKnoten).getAdjazenzListe()[n]).getId()) + " von Knoten: " + to_string((*aktKnoten).getId()) << endl;
                //(*aktKnotenNeu).getAdjazenzListe().erase((*aktKnotenNeu).getAdjazenzListe().begin()+n);
                (*aktKnoten).loescheNtesElementAusAdjazenzListe(n);
                //Hier könnte man noch verlorene Kante abspeichern, falls man die noch benutzen möchte
                n -= 1;

                /*
                cout << "AdjazenzListe von Knoten: " + to_string(v) << endl;
                for(int k = 0; k < size((*aktKnoten).getAdjazenzListe()); k++){
                    cout << "   Nachbar: " + to_string((*(*aktKnoten).getAdjazenzListe()[k]).getId()) << endl;
                }
                */
            }
        }
    }
    anzahlTeilgraphen = aktMaxZeitattribut;
    /*
    for(int k = 0; k < size((*teilgraphen[0].getKnotenMenge()[0]).getAdjazenzListe()); k++){
        cout << "Nachbar Test: " + to_string((*(*teilgraphen[0].getKnotenMenge()[0]).getAdjazenzListe()[k]).getId()) << endl;
    }
    cout << "aktMaxZeitattribut: " + to_string(aktMaxZeitattribut) << endl;
    anzahlTeilgraphen = aktMaxZeitattribut;
    cout << "anzahl Teilgraphen: " + to_string(anzahlTeilgraphen) << endl;
    */
};