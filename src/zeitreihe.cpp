#include "zeitreihe.h"
#include <vector>
using namespace std;


zeitreihe::zeitreihe(vector<int>& knoten, vector<int>& kanten){

    //Graph zu Teilgraphen

    //ZHK der Teilgraphen finden
        //Tiefensuche
            //DurchsucheNachbarn
    
    //Zeitreihe erstellen -> entweder ZHK noch analysieren, oder alle Werte werden mit ZHK bereits geliefert
};

//Input: G(knoten,kanten), Index in knoten gibt ID der Knoten an, tatsächlich gespeicherter int das Zeitattribut. Erste Kante besteht aus (kanten[0], kanten[1]) etc.
//Result: Teilknoten- und kantenmengen der einzelnen Teilgraphen, aufgeteilt nach Zeitattribut der Knoten. Die Teilknotenmengen sind als Vector bestehend aus T sets mit den 
//  entsprechenden Knoten gespeichert, wobei T die Anzahl der verschiedenen Zeitattribute in G ist. Die Teilkantenmengen sind als Vector bestehend aus T Vectoren mit den entsprechenden
//  Kanten gespeichert. 
void zeitreihe::erstelleTeilgraphenSet(vector<int>& knoten, vector<int>& kanten){

    int aktMaxZeitattribut = 0;

    for(int e = 0; e < size(kanten); e+=2){

        //für Kante (u,v): t(u) = knoten[kanten[e]]; t(v) = knoten[kanten[e+1]]

        //wird ein neues maximales Zeitattribut gefunden, dann füge den Teilgraph Knoten- und Kantensets genügend subsets hinzu, damit es genau aktMaxZeitattribut Knoten- und Kantensets gibt  
        if(knoten[kanten[e]] > aktMaxZeitattribut || knoten[kanten[e+1]] > aktMaxZeitattribut){

            int maxTmp = max(knoten[kanten[e]], knoten[kanten[e+1]]);
            int differenz = maxTmp - aktMaxZeitattribut;
            aktMaxZeitattribut = maxTmp;

            for(int i = 0; i < differenz; i++){

                set<int> s;
                teilgraphenKnoten.push_back(s);
                vector<int> v;
                teilgraphenKanten.push_back(v);
            }
        }

        //wenn Knoten u = kanten[e] und Knoten v = kanten[e+1] über dasselbe Zeitattribut vefügen, dann fügen sie dem entsprechenden Teilgraphen nr. "aktZeitattribut" hinzu. 
        //Da aktZeitattribut als Index benutzt wird, wird dieser bei insert und push_back um 1 reduziert.  
        if(knoten[kanten[e]] == knoten[kanten[e+1]]){

            int aktZeitAttribut = knoten[kanten[e]];
            teilgraphenKnoten[aktZeitAttribut-1].insert(kanten[e]);
            teilgraphenKnoten[aktZeitAttribut-1].insert(kanten[e+1]);
            teilgraphenKanten[aktZeitAttribut-1].push_back(kanten[e]);
            teilgraphenKanten[aktZeitAttribut-1].push_back(kanten[e+1]);
        }

    }
};