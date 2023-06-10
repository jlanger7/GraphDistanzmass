#include <vector>
#include <iostream>
#include "cluster.h"
using namespace std;

cluster::cluster(vector<vector<int>>* distanzMatrixIn){

    distanzMatrix = distanzMatrixIn;
};

vector<int> cluster::initPAM(int k){

    //----------------suche erstes init medoid------------
    //init tmp variablen
    int tmpMinElement = 0;
    int tmpMinDistSum = 0;
    //für jedes Objekt...
    for(int i = 0; i < size(*distanzMatrix); i++){

        //...iteriere durch die Distanzen zu jedem anderen Objekt
        //Summe der Distanzen zu anderen Objekten für Objekt i
        int tmpDistSum = 0;
        for(int j = 0; j < size((*distanzMatrix)[i]); j++){

            //füge Distanz von Objekt i zu j der Summe hinzu
            tmpDistSum += (*distanzMatrix)[i][j];
        }
        //wenn Distanzsumme von objekt i kleiner ist, als aktuelles Minimum, dann aktualisiere es
        if(i == 0 || tmpDistSum < tmpMinDistSum){
            tmpMinDistSum = tmpDistSum;
            tmpMinElement = i;
        }
    }

    //füge der Liste erstes medoid hinzu
    vector<int> initMedoids;
    initMedoids.push_back(tmpMinElement);
    //initialisiere Liste über noch nicht selektierte Objekte
    vector<int> nichtSelektierteObjekte;
    for(int i = 0; i < size(*distanzMatrix); i++){
        if(i != initMedoids[0]){
            nichtSelektierteObjekte.push_back(i);
        }
    }
    //----------------suche restliche init medoids------------
    //suche insgesamt k medoids
    while(size(initMedoids) < k){

        //Suche Objekt i, sodass die Zielfunktion bestmöglich minimiert wird, der Gewinn den das Objekt beisteuert also maximiert wird
        int tmpMaxGewinn;
        int tmpMaxObjekt;
        //Für jedes Objekt i...
        for(int i = 0; i < size(nichtSelektierteObjekte); i++){
            
            int objektI = nichtSelektierteObjekte[i];
            int summeGewinnObjektI = 0;
            //...iteriere durch jedes Objekt j
            for(int j = 0; j < size(nichtSelektierteObjekte); j++){
                
                int objektJ = nichtSelektierteObjekte[j];
                //erhöhe Gewinnsumme um die Distanz von Objekt j zum nächsten bereits gewählten Medoid abzüglich der Distanz zu Objekt i, falls Wert > 0 ist.
                int dj = getDistanzZumNaechstenGewaehltenMedoid(initMedoids, objektJ);
                summeGewinnObjektI += max((dj - (*distanzMatrix)[objektI][objektJ]), 0);
            }

            //aktualisiere das temporäre Maximum
            if(i == 0 || summeGewinnObjektI > tmpMaxGewinn){
                tmpMaxGewinn = summeGewinnObjektI;
                tmpMaxObjekt = i;
            }
        }
        //wähle Objekt welches Gewinn maximiert und verschiebe es von "nicht gewählten Objekten" zu den init Medoiden
        nichtSelektierteObjekte.erase(nichtSelektierteObjekte.begin()+tmpMaxObjekt);
        initMedoids.push_back(tmpMaxObjekt);
    }
};

int cluster::getDistanzZumNaechstenGewaehltenMedoid(vector<int> initMedoids, int objekt){

    int tmpMin;
    for(int i = 0; i < size(initMedoids); i++){

        if(i == 0 || (*distanzMatrix)[objekt][initMedoids[i]] < tmpMin){

            tmpMin = (*distanzMatrix)[objekt][initMedoids[i]];
        }
    }
    return tmpMin;
}