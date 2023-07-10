#include "zeitreihe.h"
#include "teilgraphenSet.h"
#include <vector>
#include <iostream>
using namespace std;


zeitreihe::zeitreihe(){

};

zeitreihe::zeitreihe(teilgraphenSet* teilgraphenIn){

    //Erstelle 3D-Beobachtungspunkt pro Teilgraphen im Teilgraphenset mit Zeitreihenattributwerten und füge ihn der Gesamtzeitreihe hinzu
    for(int i = 0; i < (*teilgraphenIn).getAnzahlTeilgraphen(); i++){

        vector<double> vecTmp;
        //Zeitreihenattributwerte werden schon während Tiefensuche berechnet und als Objektattribute in den Teilgraphen abgespeichert
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getAnzahlZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMaxVZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getGesVt());

        zeitreihenWerte.push_back(vecTmp);
    }
};

void zeitreihe::printZeitreihe(){

    for(int i = 0; i < size(zeitreihenWerte); i++){

        cout << "Zeitreihenschritt nr: " + to_string(i) << endl;
        cout << "   AnzahlZhk: " + to_string(zeitreihenWerte[i][0]) << endl;
        cout << "   MaxV: " + to_string(zeitreihenWerte[i][1]) << endl;
        cout << "   MinV: " + to_string(zeitreihenWerte[i][2]) << endl;
    }
};

//Berechne die diskrete Frechet-Distanz über die mehrdimensionalen Beobachtungspunkte
double zeitreihe::berechneDiskreteFrechetDistanzND(vector<vector<double>> q){

    //Hier werden die Frechet-Distanzen zwischen den mehrdimensionalen Beobachtungspunkten zweier Zeitreihen gespeichert
    double** ca = new double*[size(zeitreihenWerte)];

    //Vergleiche jeden Punkt in Zeitreihe 1 mit jedem Punkt in Zeitreihe 2
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){
            
            ca[i][j] = -1.0;
            //Berechnung der Distanz
            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswertND(q, ca, i, j);
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

//Implementierung der Fallunterscheidungen der Rekursionsgleichung des dynamischen Programms von Eiter, T., & Mannila, H. (1994). Computing Discrete Frechet Distance
double zeitreihe::berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){

        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){

        return max(caIn[i-1][j],canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i == 0 && j > 0){

        return max(caIn[i][j-1],canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i > 0 && j > 0){

        return max(min(min(caIn[i-1][j],caIn[i-1][j-1]),caIn[i][j-1]),canberraDistance(zeitreihenWerte[i],q[j]));
    }else{

        return 1000000;
    }
};

//Berechne die DTW-Distanz über die mehrdimensionalen Beobachtungspunkte
double zeitreihe::berechneDtwDistanzND(vector<vector<double>> q){

    //Hier werden die DTW-Distanzen zwischen den mehrdimensionalen Beobachtungspunkten zweier Zeitreihen gespeichert
    double** ca = new double*[size(zeitreihenWerte)];

    //Vergleiche jeden Punkt in Zeitreihe 1 mit jedem Punkt in Zeitreihe 2
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){

            ca[i][j] = -1.0;
            //Berechnung der Distanz
            ca[i][j] = berechneDtwDistanzFunktionswertND(q, ca, i, j);
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

//Implementierung der Fallunterscheidungen der Rekursionsgleichung des dynamischen Programms von Eiter, T., & Mannila, H. (1994). Computing Discrete Frechet Distance
double zeitreihe::berechneDtwDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j){

    if(caIn[i][j] > -1){

        return caIn[i][j];
    }else if(i == 0 && j == 0){

        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){

        return caIn[i-1][j] + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i == 0 && j > 0){
        
        return caIn[i][j-1] + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j > 0){
        
        return min(min(caIn[i-1][j],caIn[i-1][j-1]),caIn[i][j-1]) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else{

        return 1000000;
    }
};

//Berechnung der Canberra Distanz für mehrdimensionale Implementierungen
double zeitreihe::canberraDistance(vector<double> p, vector<double> q){

    if(size(p) != size(q)){
        throw invalid_argument( "size of input vectors not equal" );;
    }
    double dist = 0;
    for(int i = 0; i < size(p); i++){
        
        if(p[i]+q[i] == 0){
            //0, wenn p[i]+q[i]==0, da hier ausschließlich nicht-negative Werte betrachtet werden und somit dann p[i]-q[i]==0
            dist += 0;
        }else{
            dist += (double)abs(p[i]-q[i])/(double)(p[i]+q[i]);
        }
    }
    return dist;
};

//Diskrete Frechet-Distanz für 1D-Fall, analog zu mehrdimensionalem Fall und Ersetzung Canberra-Distanz mit absoluter Differenz
double zeitreihe::berechneDiskreteFrechetDistanz1D(vector<vector<double>> q, int attribut){

    double** ca = new double*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){
           
            ca[i][j] = -1;
            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswert1D(q, ca, i, j, attribut);
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

//Diskrete Frechet-Distanz für 1D-Fall, analog zu mehrdimensionalem Fall und Ersetzung Canberra-Distanz mit absoluter Differenz
double zeitreihe::berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<double>> q, double** caIn, int i , int j, int attribut){

    if(caIn[i][j] > -1){

        return caIn[i][j];
    }else if(i == 0 && j == 0){

        return d(zeitreihenWerte[i], q[j], attribut);
    }else if(i > 0 && j == 0){

        return max(caIn[i-1][j],d(zeitreihenWerte[i], q[j], attribut));
    }else if(i == 0 && j > 0){

        return max(caIn[i][j-1],d(zeitreihenWerte[i], q[j], attribut));
    }else if(i > 0 && j > 0){

        return max(min(min(caIn[i-1][j],caIn[i-1][j-1]),caIn[i][j-1]),d(zeitreihenWerte[i], q[j], attribut));
    }else{

        return 1000000;
    }
};

//DTW-Distanz für 1D-Fall, analog zu mehrdimensionalem Fall und Ersetzung Canberra-Distanz mit absoluter Differenz
double zeitreihe::berechneDtwDistanz1D(vector<vector<double>> q, int attribut){

    double** ca = new double*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){
            
            ca[i][j] = -1;
            ca[i][j] = berechneDtwDistanzFunktionswert1D(q, ca, i, j, attribut);
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

//DTW-Distanz für 1D-Fall, analog zu mehrdimensionalem Fall und Ersetzung Canberra-Distanz mit absoluter Differenz
double zeitreihe::berechneDtwDistanzFunktionswert1D(vector<vector<double>> q, double** caIn, int i , int j, int attribut){

    if(caIn[i][j] > -1){
        
        return caIn[i][j];
    }else if(i == 0 && j == 0){

        return d(zeitreihenWerte[i], q[j], attribut);
    }else if(i > 0 && j == 0){

        return caIn[i-1][j] + d(zeitreihenWerte[i], q[j], attribut);
    }else if(i == 0 && j > 0){
        
        return caIn[i][j-1] + d(zeitreihenWerte[i], q[j], attribut);
    }else if(i > 0 && j > 0){
        
        return min(min(caIn[i-1][j],caIn[i-1][j-1]),caIn[i][j-1]) + d(zeitreihenWerte[i], q[j], attribut);
    }else{

        return 1000000;
    }
};

//Genutzte Distanzfunktion für die 1D- Implementierungen
double zeitreihe::d(vector<double> p, vector<double> q, int attribut){

    return abs(p[attribut] - q[attribut]);
}