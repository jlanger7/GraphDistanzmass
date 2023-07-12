#include <vector>
#include <iostream>
#include "pamClustering.h"
using namespace std;

pamClustering::pamClustering(vector<vector<double>>* distanzMatrixIn, int kIn){

    distanzMatrix = distanzMatrixIn;
    k = kIn;
};

//Implementierung BUILD-Phase von Kaufman, L., & Rousseeuw, P. J. (n. d.). Partitioning Around Medoids (Program PAM).
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
        //wenn Distanzsumme von objekt i kleiner ist, als aktuelles Minimum, dann aktualisiere das Minimum
        if(i == 0 || tmpDistSum < tmpMinDistSum){
            tmpMinDistSum = tmpDistSum;
            tmpMinElement = i;
        }
    }

    //füge der Liste ersten Medoiden hinzu
    medoids.push_back(tmpMinElement);
    //initialisiere Liste über noch nicht selektierte Objekte
    for(int i = 0; i < size(*distanzMatrix); i++){
        if(i != medoids[0]){
            nichtSelektierteObjekte.push_back(i);
        }
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
                summeGewinnObjektI += max((dj - (*distanzMatrix)[objektI][objektJ]), (double)0);
            }
            //aktualisiere das temporäre Maximum
            if(i == 0 || summeGewinnObjektI > tmpMaxGewinn){
                tmpMaxGewinn = summeGewinnObjektI;
                tmpMaxObjektIndex = i;
            }
        }
        //wähle Objekt welches Gewinn maximiert und verschiebe es von "nicht gewählten Objekten" zu den init Medoiden
        medoids.push_back(nichtSelektierteObjekte[tmpMaxObjektIndex]);
        nichtSelektierteObjekte.erase(nichtSelektierteObjekte.begin()+tmpMaxObjektIndex);
    }
};

double pamClustering::getDistanzZumNaechstenGewaehltenMedoid(int objekt){

    double tmpMin;
    for(int i = 0; i < size(medoids); i++){

        if(i == 0 || (*distanzMatrix)[objekt][medoids[i]] < tmpMin){

            tmpMin = (*distanzMatrix)[objekt][medoids[i]];
        }
    }
    return tmpMin;
};

//Implementierung SWAP-Phase von Kaufman, L., & Rousseeuw, P. J. (n. d.). Partitioning Around Medoids (Program PAM).
vector<vector<int>> pamClustering::berechneClustering(){

    //Berechne initiale Medoids
    init();

    //Führe maximal 99 Iterationen zur Verfeinerung der Medoids-Wahl durch
    bool konvergiert = false;
    int iterationen = 0;
    while(!konvergiert && iterationen < 99){

        double minKosten;
        int minI;
        int minH;
        //Prüfe Effekt eines Tausches von aktuell gewähltem Medoiden i mit...
        for(int i = 0; i < size(medoids); i++){
            //...aktuell nicht gewähltem Objekt h:
            for(int h = 0; h < size(nichtSelektierteObjekte); h++){

                //Initialisiere den Beitrag, den der Tausch von i und h zur Verbesserung der Kostenfunktion beitragen würde
                double sumTauschIH = 0;
                //Prüfe Effekt eines Tausches von i und h auf konkretes nicht-gewähltes Objekt j, das nicht Objekt h ist
                for(int j = 0; j < size(nichtSelektierteObjekte); j++){
                    
                    //Prüfe nur, wenn Objekt j nicht Objekt h ist
                    if(j!=h){

                        //Effekt des Tausches von i und h auf j
                        double c;
                        //Distanz von Objekt j zu dem aktuell gewähltem Meodiden, der ihm am nächsten ist
                        double distanzNaechsterMedoid = getDistanzZumNaechstenGewaehltenMedoid(nichtSelektierteObjekte[j]);

                        //Wenn i und h von j weiter entfernt sind, als sein aktuell nächster Medoid, dann hat der Tausch von i und h keinen Effekt auf j
                        if((*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] > distanzNaechsterMedoid && 
                                (*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] > distanzNaechsterMedoid){

                            c = 0;
                        }
                        //Wenn i nicht weiter entfernt von j ist, als sein aktuell nächster Medoid...
                        if((*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] <= distanzNaechsterMedoid){

                            //Distanz von Objekt j zu dem aktuell gewähltem Meodiden, der ihm am zweit-nächsten ist
                            double distanzZweitNaechsterMedoid = getDistanzZumZweitNaechstenGewaehltenMedoid(nichtSelektierteObjekte[j]);
                            
                            //Objekt j ist näher an h, als am zweit-nächsten Medoiden
                            if((*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] < distanzZweitNaechsterMedoid){
                                
                                //Bei Tausch von i und h wäre h der neue nächste Medoid von j, somit wäre der Beitrag d(j,h)-d(j,i) 
                                c = (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] - (*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]];
                            }
                            //Objekt j ist nicht näher an h, als am zweit-nächsten Medoiden
                            else{
                                
                                //Bei Tausch von i und h wäre der aktuell zweit-nächste Medoid der neue nächste Medoid von j, somit wäre der Beitrag: 
                                //d(j, aktuell zweit-nächster Medoid)-d(j, aktuell nächster Medoid)
                                c = distanzZweitNaechsterMedoid - distanzNaechsterMedoid;
                            }
                        }
                        //Objekt ist weiter weg von i als von mindestens einem anderen aktuellen Medoiden (i ist also nicht der nächste Medoid von j), aber näher an h,
                        //als an allen anderen aktuellen medoiden -> h wäre also neuer Medoid bei Tausch von i und h
                        if((*distanzMatrix)[nichtSelektierteObjekte[j]][medoids[i]] > distanzNaechsterMedoid && 
                                (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] <= distanzNaechsterMedoid){
                                
                            //Beitrag wäre die Differenz, um die h näher an j ist, als j's aktueller nächster Medoid
                            c = (*distanzMatrix)[nichtSelektierteObjekte[j]][nichtSelektierteObjekte[h]] - distanzNaechsterMedoid;
                        }
                        //Addiere Beitrag von aktuellem j zu Beiträgen aller j's bei Tausch von i und h
                        sumTauschIH += c;
                    }
                }
                //Aktualisiere besten Tausch
                if((i == 0 && h == 0) || (sumTauschIH < minKosten)){
                    
                    minKosten = sumTauschIH;
                    minI = i;
                    minH = h;
                }
            }
        }
        //Wenn bester Tausch der aktuellen Iteration die Gesamtkostenfunktion verringert, dann führe ihn durch
        if(minKosten < 0){
            
            int neuerMedoid = nichtSelektierteObjekte[minH];
            int neuesNichtselektierteObjekt = medoids[minI];
            nichtSelektierteObjekte.erase(nichtSelektierteObjekte.begin()+minH);
            nichtSelektierteObjekte.push_back(neuesNichtselektierteObjekt);
            medoids.erase(medoids.begin()+minI);
            medoids.push_back(neuerMedoid);
        }else{
            konvergiert = true;
            if(true){ cout << "Ende nach Iteration Nr. " + to_string(iterationen) << endl;}
        }
        
        iterationen += 1;
    }
    cout << "   Clustering berechnet" << endl;
    
    //Berechne die Zuordnung eines jeden nicht gewählten Objekts zu seinem nächsten Medoiden/ Cluster
    berechneZuordnungZuCluster();

    return zuordnungZuCluster;
};

void pamClustering::berechneZuordnungZuCluster(){

    wertKostenFunktion = 0;

    //Füge "zuordnungZuCluster" pro Medoid einen Vector hinzu, der zunächst nur den jeweiligen Medoiden enthält und das Cluster repräsentiert
    for(int i = 0; i < k; i++){
        vector<int> clusterMitglieder;
        clusterMitglieder.push_back(medoids[i]);
        zuordnungZuCluster.push_back(clusterMitglieder);
    }

    //Finde für jedes Objekt den nächsten Medoiden und somit sein Cluster
    for(int i = 0; i < size(*distanzMatrix); i++){
        
        int minMedoidIndex;
        double minMedoidDist;
        //Prüfe jeden Medoiden
        for(int m = 0; m < size(medoids); m++){

            double aktDistanz = ((double)(*distanzMatrix)[i][medoids[m]])*((double)(*distanzMatrix)[i][medoids[m]]);
            //Aktualisiere den nächsten Medoiden
            if(m == 0 || aktDistanz < minMedoidDist){

                minMedoidDist = aktDistanz;
                minMedoidIndex = m;
            }
        }
        //Addiere Wert zur Gesamtkostenfunktion hinzu
        wertKostenFunktion += minMedoidDist;

        //Füge Objekt zu seinem Cluster hinzu
        if(i != zuordnungZuCluster[minMedoidIndex][0]){
            
            zuordnungZuCluster[minMedoidIndex].push_back(i);
        }
    }
};

double pamClustering::getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt){

    //Initialisiere Distanz zum nächsten und zweit-nächstem Medoiden
    double tmpMin;
    double tmpMin2;
    if((*distanzMatrix)[objekt][medoids[0]] <= (*distanzMatrix)[objekt][medoids[1]]){
        tmpMin = (*distanzMatrix)[objekt][medoids[0]];
        tmpMin2 = (*distanzMatrix)[objekt][medoids[1]];
    }else{
        tmpMin = (*distanzMatrix)[objekt][medoids[1]];
        tmpMin2 = (*distanzMatrix)[objekt][medoids[0]];
    }
    
    //Prüfe jeden Medoiden, ob er näher am Eingabeobjekt ist als der aktuell nächste oder zweit-nächste Medoid
    for(int i = 0; i < size(medoids); i++){

        //Wenn aktuell betrachteter Medoid i näher am Eingabeobjekt ist, als der bisher nächste Medoid, 
        //dann wird der nächste Medoid durch i ersetzt und der bisher nächste Medoid wird zum zweit-nächsten Medoid
        if((*distanzMatrix)[objekt][medoids[i]] < tmpMin){

            tmpMin2 = tmpMin;
            tmpMin = (*distanzMatrix)[objekt][medoids[i]];
        }
        //Wenn aktuell betrachteter Medoid i nicht näher am Eingabeobjekt ist, als der bisher nächste Medoid, 
        //aber näher als der bisher zweit-nächste Medoid, dann ersetze den bisher zweit-nächsten Medoiden durch i
        else if((*distanzMatrix)[objekt][medoids[i]] < tmpMin2 && (*distanzMatrix)[objekt][medoids[i]] > tmpMin){
            tmpMin2 = (*distanzMatrix)[objekt][medoids[i]];
        }
    }
    return tmpMin2;
};