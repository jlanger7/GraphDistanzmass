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

void graph::modifizierteTiefensucheNeu(){

    int aktNrZhks = 0, tmpMaxV = 0, tmpMeanV = 0;
    for(int v = 0; v < size(knotenMenge); v++){
        
        knoten* startKnoten = knotenMenge[v];
        if((*startKnoten).getZhkNr() == -1){

            aktNrZhks += 1;
            //cout << "       mod.TiefensucheNeu Anzahl Zhks: " + to_string(aktNrZhks) << endl;
            (*startKnoten).setZhkNr(aktNrZhks);
            vector<knoten*> warteschlange;
            warteschlange.push_back(startKnoten);
            int tmpNrV = 1;

            while(size(warteschlange) > 0){

                knoten* aktKnoten = warteschlange.back();
                warteschlange.pop_back();
                /*for(int i = 0; i< size(warteschlange); i++){
                    cout << "IDs in warteschlange vor loeschen: " + to_string((*warteschlange[i]).getId()) << endl;
                }*/
                //warteschlange.erase(warteschlange.begin());
                /*for(int i = 0; i< size(warteschlange); i++){
                    cout << "IDs in warteschlange nach loeschen: " + to_string((*warteschlange[i]).getId()) << endl;
                }if(size(warteschlange) == 0){
                    cout << "IDs in warteschlange nach loeschen: empty" << endl;
                }*/
                
                vector<knoten*> adjazenzliste = (*aktKnoten).getAdjazenzListe();
                for(int u = 0; u < size(adjazenzliste); u++){
                    
                    if((*adjazenzliste[u]).getZhkNr() == -1){

                        (*adjazenzliste[u]).setZhkNr(aktNrZhks);
                        warteschlange.push_back(adjazenzliste[u]);
                        tmpNrV += 1;
                    }
                }
            }
            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            tmpMeanV += tmpNrV;
            //cout << "       DONE mod.TiefensucheNeu Anzahl Zhks: " + to_string(aktNrZhks) << endl;
        }
    }
    anzahlZhk = aktNrZhks;
    maxVZhk = tmpMaxV;
    meanVZhk = tmpMeanV/anzahlZhk;
    cout << "   DONE mod.TiefensucheNeu" << endl;
};

void graph::modifizierteTiefensuche(){

    int aktNrZhks = 0, tmpMaxV = 0, tmpMeanV = 0;
    for(int v = 0; v < size(knotenMenge); v++){
        
        knoten& aktKnoten = (*knotenMenge[v]);
        if(aktKnoten.getZhkNr() == -1){

            aktNrZhks += 1;
            cout << "       mod.Tiefensuche Anzahl Zhks: " + to_string(aktNrZhks) << endl;
            aktKnoten.setZhkNr(aktNrZhks);
            int tmpNrV = 1;
            //cout << "Parameter vor Aufruf druchsucheNachbarn. aktNrZhks: " + to_string(aktNrZhks) + " tmpNrV: " + to_string(tmpNrV) << endl;
            durchsucheNachbarn(aktKnoten, aktKnoten.getAdjazenzListe(), aktNrZhks, tmpNrV);

            if(tmpMaxV == 0 || tmpNrV > tmpMaxV){
                tmpMaxV = tmpNrV;
            }
            tmpMeanV += tmpNrV;
            cout << "       DONE mod.Tiefensuche Anzahl Zhks: " + to_string(aktNrZhks) << endl;
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
    if(v.getId() > 500000000){cout << "           enter durchsucheNachbarn vau: " + to_string(v.getId()) << endl;}
    for(int u = 0; u < size(adjazenzListe); u++){

        if(v.getId() > 500000000){cout << "           Adjazenz vau: " + to_string(v.getId()) + " Nachbar Zhk zuordnung " + to_string((*adjazenzListe[u]).getZhkNr()) << endl;}
        if((*adjazenzListe[u]).getZhkNr() == -1){
            
            (*adjazenzListe[u]).setZhkNr(aktNrZhks);
            if(v.getId() > 500000000){cout << "           was here: " + to_string(__LINE__) << endl;}
            tmpNrV += 1;
            if(v.getId() > 500000000){cout << "           was here: " + to_string(__LINE__) << endl;}
            durchsucheNachbarn((*adjazenzListe[u]), (*adjazenzListe[u]).getAdjazenzListe(), aktNrZhks, tmpNrV);
        }
    }
};

void graph::addKnoten(knoten* knotenIn){

    knotenMenge.push_back(knotenIn);
};

