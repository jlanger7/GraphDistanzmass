#include <vector>
#include <iostream>
#include "pamClustering.h"
using namespace std;

pamClustering::pamClustering(vector<vector<int>>* distanzMatrixIn, int kIn){

    distanzMatrix = distanzMatrixIn;
    k = kIn;
};

void pamClustering::init(){

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
    medoids.push_back(tmpMinElement);
    //initialisiere Liste über noch nicht selektierte Objekte
    for(int i = 0; i < size(*distanzMatrix); i++){
        if(i != medoids[0]){
            nichtSelektierteObjekte.push_back(i);
        }
    }
    cout << "noch nicht gewaehlte objekte Beginn: " << endl;
        for(int z = 0; z < size(nichtSelektierteObjekte); z++){
            cout << "Objekt: " + to_string(nichtSelektierteObjekte[z])<< endl;
        }
    //----------------suche restliche init medoids------------
    //suche insgesamt k medoids
    while(size(medoids) < k){

        //Suche Objekt i, sodass die Zielfunktion bestmöglich minimiert wird, der Gewinn den das Objekt beisteuert also maximiert wird
        int tmpMaxGewinn;
        int tmpMaxObjektIndex;
        //Für jedes Objekt i...
        for(int i = 0; i < size(nichtSelektierteObjekte); i++){
            
            int objektI = nichtSelektierteObjekte[i];
            int summeGewinnObjektI = 0;
            //...iteriere durch jedes Objekt j
            for(int j = 0; j < size(nichtSelektierteObjekte); j++){
                
                int objektJ = nichtSelektierteObjekte[j];
                //erhöhe Gewinnsumme um die Distanz von Objekt j zum nächsten bereits gewählten Medoid abzüglich der Distanz zu Objekt i, falls Wert > 0 ist.
                int dj = getDistanzZumNaechstenGewaehltenMedoid(objektJ);
                cout << "Distanz von Objekt " + to_string(objektJ) + " zu Objekt " + to_string(objektI) + ": " + to_string((*distanzMatrix)[objektI][objektJ])<< endl;
                summeGewinnObjektI += max((dj - (*distanzMatrix)[objektI][objektJ]), 0);
            }
            cout << "Summe Gewinn von Objekt " + to_string(objektI) + ": " + to_string(summeGewinnObjektI)<< endl;
            //aktualisiere das temporäre Maximum
            if(i == 0 || summeGewinnObjektI > tmpMaxGewinn){
                tmpMaxGewinn = summeGewinnObjektI;
                tmpMaxObjektIndex = i;
            }
        }
        //wähle Objekt welches Gewinn maximiert und verschiebe es von "nicht gewählten Objekten" zu den init Medoiden
        medoids.push_back(nichtSelektierteObjekte[tmpMaxObjektIndex]);
        cout << "maxObjekt: " + to_string(nichtSelektierteObjekte[tmpMaxObjektIndex]) + " zu medoids hinzugefuegt" << endl;
        nichtSelektierteObjekte.erase(nichtSelektierteObjekte.begin()+tmpMaxObjektIndex);
        cout << "noch nicht gewaehlte objekte jetzt: " << endl;
        for(int z = 0; z < size(nichtSelektierteObjekte); z++){
            cout << "Objekt: " + to_string(nichtSelektierteObjekte[z])<< endl;
        }
    }
};

int pamClustering::getDistanzZumNaechstenGewaehltenMedoid(int objekt){

    int tmpMin;
    for(int i = 0; i < size(medoids); i++){

        if(i == 0 || (*distanzMatrix)[objekt][medoids[i]] < tmpMin){

            tmpMin = (*distanzMatrix)[objekt][medoids[i]];
        }
    }
    //cout << "Distanz von Objekt " + to_string(objekt) + " zu medoid: " + to_string(tmpMin)<< endl;
    return tmpMin;
};

vector<int> pamClustering::berechneClustering(){

    init();
    cout << "init abgeschlossen" << endl;
    bool konvergiert = false;
    while(!konvergiert){
        cout << "Durchlauf while" << endl;
        int minKosten;
        int minI;
        int minH;
        for(int i = 0; i < size(medoids); i++){
            
            cout << "berechneClustering for medoid nr: " + to_string(i) + " objekt: " + to_string(medoids[i]) << endl;
            cout << "noch nicht gewaehlte objekte jetzt: " << endl;
            for(int z = 0; z < size(nichtSelektierteObjekte); z++){
                cout << "Objekt: " + to_string(nichtSelektierteObjekte[z])<< endl;
            }
            cout << "medoids objekte jetzt: " << endl;
            for(int z = 0; z < size(medoids); z++){
                cout << "Objekt: " + to_string(medoids[z])<< endl;
            }

            for(int h = 0; h < size(nichtSelektierteObjekte); h++){

                cout << "   ha: " + to_string(h) << endl;
                int sumTauschIH = 0;
                for(int j = 0; j < size(nichtSelektierteObjekte); j++){
                    cout << "   jot: " + to_string(j) << endl;
                    int c;
                    int maxDistanzZuEinemAnderenMedoid = getDistanzZumWeitestenGewaehltenMedoidAusserI(nichtSelektierteObjekte[j], medoids[i]);
                    if((*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] > maxDistanzZuEinemAnderenMedoid && 
                            (*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] > maxDistanzZuEinemAnderenMedoid){
                        
                        c = 0;
                        cout << "       if1" << endl;
                    }
                    if((*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] <= maxDistanzZuEinemAnderenMedoid){

                        int distanzZweitNaechsterMedoid = getDistanzZumZweitNaechstenGewaehltenMedoid(nichtSelektierteObjekte[j]);
                        if((*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] < distanzZweitNaechsterMedoid){
                            
                            c = (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] - (*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]];
                            cout << "       if2" << endl;
                        }else{
                            
                            c = distanzZweitNaechsterMedoid - getDistanzZumNaechstenGewaehltenMedoid(nichtSelektierteObjekte[j]);
                            cout << "       if3 ce: " + to_string(c) << endl;
                        }
                    }
                    int distanzNaechsterMedoid = getDistanzZumNaechstenGewaehltenMedoid(nichtSelektierteObjekte[j]);
                    if((*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] > distanzNaechsterMedoid && 
                            (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] < distanzNaechsterMedoid){
                              
                        c = (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] - distanzNaechsterMedoid;
                        cout << "       if4" << endl;
                    }
                    
                    sumTauschIH += c;
                }
                if((i == 0 && h == 0) || (sumTauschIH < minKosten)){
                    
                    minKosten = sumTauschIH;
                    minI = i;
                    minH = h;
                }
            }
        }
        cout << "minKosten: " + to_string(minKosten) << endl;
        if(minKosten < 0){
            
            int neuerMedoid = nichtSelektierteObjekte[minH];
            int neuesNichtselektierteObjekt = medoids[minI];
            cout << "neuer medoid " + to_string(neuerMedoid) << endl;
            cout << "neues objekt " + to_string(neuesNichtselektierteObjekt) << endl;
            nichtSelektierteObjekte.erase(nichtSelektierteObjekte.begin()+minH);
            nichtSelektierteObjekte.push_back(neuesNichtselektierteObjekt);
            medoids.erase(medoids.begin()+minI);
            medoids.push_back(neuerMedoid);
        }else{
            konvergiert = true;
            cout << "stopp setzen fuer while" << endl;
        }
    }
    return medoids;
};

int pamClustering::getDistanzZumWeitestenGewaehltenMedoidAusserI(int objekt, int medoidI){

    int tmpMax = 0;
    for(int i = 0; i < size(medoids); i++){

        if((*distanzMatrix)[objekt][medoids[i]] > tmpMax && medoids[i]!= medoidI){

            tmpMax = (*distanzMatrix)[objekt][medoids[i]];
        }
    }
    //cout << "Distanz von Objekt " + to_string(objekt) + " zu medoid: " + to_string(tmpMax)<< endl;
    return tmpMax;
};

int pamClustering::getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt){

    int tmpMin;
    int tmpMin2;
    if((*distanzMatrix)[objekt][medoids[0]] <= (*distanzMatrix)[objekt][medoids[1]]){
        tmpMin = (*distanzMatrix)[objekt][medoids[0]];
        tmpMin2 = (*distanzMatrix)[objekt][medoids[1]];
    }else{
        tmpMin = (*distanzMatrix)[objekt][medoids[1]];
        tmpMin2 = (*distanzMatrix)[objekt][medoids[0]];
    }
    
    for(int i = 0; i < size(medoids); i++){

        if((*distanzMatrix)[objekt][medoids[i]] < tmpMin){

            tmpMin2 = tmpMin;
            tmpMin = (*distanzMatrix)[objekt][medoids[i]];
        }else if((*distanzMatrix)[objekt][medoids[i]] < tmpMin2 && (*distanzMatrix)[objekt][medoids[i]] > tmpMin){
            tmpMin2 = (*distanzMatrix)[objekt][medoids[i]];
        }
    }
    cout << "Distanz von Objekt " + to_string(objekt) + " zu zweit naechstem medoid: " + to_string(tmpMin2)<< endl;
    return tmpMin2;
};