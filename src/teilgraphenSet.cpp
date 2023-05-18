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
};

void teilgraphenSet::erstelleTeilgraphenSet(){

    int aktMaxZeitattribut = 0;
    vector<knoten> gesamtKnotenMenge = (*gesamtGraph).getKnotenMenge();

    for(int v = 0; v < size(gesamtKnotenMenge); v++){
        
        cout << "V nr: " + to_string(v) << endl;
        knoten aktKnoten = gesamtKnotenMenge[v];
        int zeitAttributAktKnoten = aktKnoten.getZeitAttribut();
        if(zeitAttributAktKnoten > aktMaxZeitattribut){

            int differenz = zeitAttributAktKnoten - aktMaxZeitattribut;
            cout << "differenz: " + to_string(differenz) << endl;
            aktMaxZeitattribut = zeitAttributAktKnoten;
            for(int i = 0; i < differenz; i++){

                graph* teilgraph = new graph();
                teilgraphen.push_back(teilgraph);
            }
        }
        (*teilgraphen[zeitAttributAktKnoten-1]).addKnoten(aktKnoten);
        (*teilgraphen[zeitAttributAktKnoten-1]).name = "derGraph";
        cout << (*teilgraphen[zeitAttributAktKnoten-1]).name << endl;
        knoten* aktKnotenNeu = &(*teilgraphen[zeitAttributAktKnoten-1]).getKnotenMenge()[size((*teilgraphen[zeitAttributAktKnoten-1]).getKnotenMenge())-1];
        for(int n = 0; n < size(aktKnoten.getAdjazenzListe()); n++){

            if(!((*(*aktKnotenNeu).getAdjazenzListe()[n]).getZeitAttribut() == zeitAttributAktKnoten)){
                cout << "loesche: " + to_string((*(*aktKnotenNeu).getAdjazenzListe()[n]).getId()) + " von Knoten: " + to_string((*aktKnotenNeu).getId()) << endl;
                //(*aktKnotenNeu).getAdjazenzListe().erase((*aktKnotenNeu).getAdjazenzListe().begin()+n);
                (*aktKnotenNeu).loescheNtesElementAusAdjazenzListe(n);
                (*aktKnotenNeu).name = "derKnoten"; 
                cout << (*aktKnotenNeu).name  << endl;
                //n -= 1;

                cout << "AdjazenzListe von Knoten: " + to_string(v) << endl;
                for(int k = 0; k < size((*aktKnotenNeu).getAdjazenzListe()); k++){
                    cout << "   Nachbar: " + to_string((*(*aktKnotenNeu).getAdjazenzListe()[k]).getId()) << endl;
                }
            }
        }
    }

    for(int k = 0; k < size((*teilgraphen[0]).getKnotenMenge()[0].getAdjazenzListe()); k++){
        cout << "Nachbar Test: " + to_string((*(*teilgraphen[0]).getKnotenMenge()[0].getAdjazenzListe()[k]).getId()) << endl;
    }
cout << "aktMaxZeitattribut: " + to_string(aktMaxZeitattribut) << endl;
    anzahlTeilgraphen = aktMaxZeitattribut;
    cout << "anzahl Teilgraphen: " + to_string(anzahlTeilgraphen) << endl;
};