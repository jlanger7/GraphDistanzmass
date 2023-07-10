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

//Hier wird der Eingabegraph pro Zeitschritt in die Teilgraphen eingeteilt
void teilgraphenSet::erstelleTeilgraphenSet(){

    //Knotenmenge des Eingabegraphen
    vector<knoten*> gesamtKnotenMenge = (*gesamtGraph).getKnotenMenge();
    int aktMaxZeitattribut = 0;

    //Iteriere über alle Knoten und füge ihn dem den Zeitschritt entsprechenden Teilgraphen hinzu, bzw. erstelle diesen Teilgraphen, falls er noch nicht existiert
    for(int v = 0; v < size(gesamtKnotenMenge); v++){
        
        //Nehme den aktuell betrachteten Knoten und seinen Zeitschritt
        knoten* aktKnoten = gesamtKnotenMenge[v];
        int zeitAttributAktKnoten = (*aktKnoten).getZeitAttribut();

        //Wenn Zeitschritt vom aktuellen Knoten größer ist, als der größte bisher betrachtete, dann initialisiere Teilgraphen für die Differenz in den Zeitschritten
        if(zeitAttributAktKnoten > aktMaxZeitattribut){

            int differenz = zeitAttributAktKnoten - aktMaxZeitattribut;
            aktMaxZeitattribut = zeitAttributAktKnoten;
            for(int i = 0; i < differenz; i++){

                graph teilgraph;
                //Füge neuen Teilgraphen der Teilgraphenliste hinzu
                teilgraphen.push_back(teilgraph);
            }
        }
        //Füge aktuellen Knoten dem Zeitschritt entsprechenden Teilgraphen in der Teilgraphenliste hinzu 
        teilgraphen[zeitAttributAktKnoten-1].addKnoten(aktKnoten);

        //Lösche alle Knoten aus der Adjazenzliste, bei denen der Zeitschritt nicht mit dem des aktuellen Knotens übereinstimmt, damit Kanten nur zwischen Knoten mit gleichem
        //Zeitschritt existieren
        for(int n = 0; n < size((*aktKnoten).getAdjazenzListe()); n++){

            if(!((*(*aktKnoten).getAdjazenzListe()[n]).getZeitAttribut() == zeitAttributAktKnoten)){
                
                (*aktKnoten).loescheNtesElementAusAdjazenzListe(n);
                //Hier könnte man noch verlorene Kanten abspeichern, falls man die noch nutzen möchte
                n -= 1;
            }
        }
    }
    anzahlTeilgraphen = aktMaxZeitattribut;
};