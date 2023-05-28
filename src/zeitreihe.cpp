#include "zeitreihe.h"
#include "teilgraphenSet.h"
#include <vector>
#include <iostream>
using namespace std;


zeitreihe::zeitreihe(){

};

zeitreihe::zeitreihe(teilgraphenSet* teilgraphenIn){

    for(int i = 0; i < (*teilgraphenIn).getAnzahlTeilgraphen(); i++){

        vector<int> vecTmp;
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getAnzahlZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMaxVZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMinVZhk());
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

int zeitreihe::berechneDiskreteFrechetDistanz(vector<vector<int>> q){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswert(q, ca, i, j);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDiskreteFrechetDistanzFunktionswert(vector<vector<int>> q, int** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return d(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        return max(berechneDiskreteFrechetDistanzFunktionswert(q, caIn, i-1, j),d(zeitreihenWerte[i],q[j]));
    }else if(i == 0 && j > 0){
        return max(berechneDiskreteFrechetDistanzFunktionswert(q, caIn, i, j-1),d(zeitreihenWerte[i],q[j]));
    }else if(i > 0 && j > 0){
        return max(min(min(berechneDiskreteFrechetDistanzFunktionswert(q, caIn, i-1, j),berechneDiskreteFrechetDistanzFunktionswert(q, caIn, i-1, j-1)),
            berechneDiskreteFrechetDistanzFunktionswert(q, caIn, i, j-1)),d(zeitreihenWerte[i],q[j]));
    }else{
        return 1000000;
    }
};

int zeitreihe::berechneDtwDistanz(vector<vector<int>> q){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDtwDistanzFunktionswert(q, ca, i, j);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDtwDistanzFunktionswert(vector<vector<int>> q, int** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return d(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        return berechneDtwDistanzFunktionswert(q, caIn, i-1, j) + d(zeitreihenWerte[i],q[j]);
    }else if(i == 0 && j > 0){
        return berechneDtwDistanzFunktionswert(q, caIn, i, j-1) + d(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j > 0){
        return min(min(berechneDtwDistanzFunktionswert(q, caIn, i-1, j),berechneDtwDistanzFunktionswert(q, caIn, i-1, j-1)),
            berechneDtwDistanzFunktionswert(q, caIn, i, j-1)) + d(zeitreihenWerte[i],q[j]);
    }else{
        return 1000000;
    }
};

int zeitreihe::d(vector<int> p, vector<int> q){

    int dist = 0;
    for(int i = 0; i < size(p); i++){
        dist = dist + abs(p[i]-q[i]);
    }
    return dist;
};