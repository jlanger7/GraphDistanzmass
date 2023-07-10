#include <vector>
#include <iostream>
#include "knoten.h"
#include "graph.h"
using namespace std;

graph::graph(){

};

graph::graph(vector<knoten*>& knotenMengeIn){

    knotenMenge = knotenMengeIn;
};

void graph::modifizierteTiefensucheNeu(){

    //Init Zählvariablen um Anzahl Zhk, max. V aller Zhk und Summe V aller Zhk für spätere Zeitreihenattribute zu ermitteln
    int aktNrZhks = 0, tmpMaxV = 0, tmpGesV = 0;

    //Iteriere über jeden Knoten
    for(int v = 0; v < size(knotenMenge); v++){
        
        
        knoten* startKnoten = knotenMenge[v];
        //Wenn aktueller Knoten noch nicht Teil einer bereits entdeckten Zhk ist, dann starte Tiefensuche
        if((*startKnoten).getZhkNr() == -1){

            //Neu entdeckte Zhk
            aktNrZhks += 1;
            (*startKnoten).setZhkNr(aktNrZhks);
           
           //Füge v zur Warteschlange hinzu, für iterative Implementierung der Tiefensuche
            vector<knoten*> warteschlange;
            warteschlange.push_back(startKnoten);
            int tmpNrV = 1;

            while(size(warteschlange) > 0){

                //Extrahiere zuletzt zur Warteschlange hinzugefügten Knoten
                knoten* aktKnoten = warteschlange.back();
                warteschlange.pop_back();
                
                //Iteriere durch Adjazenzliste von aktuell betrachtetem Knoten
                vector<knoten*> adjazenzliste = (*aktKnoten).getAdjazenzListe();
                for(int u = 0; u < size(adjazenzliste); u++){
                    
                    //Wenn aktuell betrachteter Nachbar noch nicht Teil einer bisher entdeckten Zhk, dann füge ihn zur Warteschlange hinzu
                    if((*adjazenzliste[u]).getZhkNr() == -1){

                        (*adjazenzliste[u]).setZhkNr(aktNrZhks);
                        warteschlange.push_back(adjazenzliste[u]);
                        tmpNrV += 1;
                    }
                }
            }
            //Aktualisiere max. V aller Zhk
            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            //Aktualisiere Summe V aller Zhk
            tmpGesV += tmpNrV;
        }
    }
    //Setze Objektvariablen für spätere Zeitreihenattribute
    anzahlZhk = aktNrZhks;
    maxVZhk = tmpMaxV;
    gesVt = tmpGesV;
};

void graph::addKnoten(knoten* knotenIn){

    knotenMenge.push_back(knotenIn);
};