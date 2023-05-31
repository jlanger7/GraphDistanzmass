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

int zeitreihe::berechneDiskreteFrechetDistanzND(vector<vector<int>> q){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswertND(q, ca, i, j);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<int>> q, int** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        return max(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j),canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i == 0 && j > 0){
        return max(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i, j-1),canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i > 0 && j > 0){
        return max(min(min(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j),berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j-1)),
            berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i, j-1)),canberraDistance(zeitreihenWerte[i],q[j]));
    }else{
        return 1000000;
    }
};

int zeitreihe::berechneDtwDistanzND(vector<vector<int>> q){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDtwDistanzFunktionswertND(q, ca, i, j);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDtwDistanzFunktionswertND(vector<vector<int>> q, int** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        return berechneDtwDistanzFunktionswertND(q, caIn, i-1, j) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i == 0 && j > 0){
        return berechneDtwDistanzFunktionswertND(q, caIn, i, j-1) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j > 0){
        return min(min(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1)),
            berechneDtwDistanzFunktionswertND(q, caIn, i, j-1)) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else{
        return 1000000;
    }
};

int zeitreihe::canberraDistance(vector<int> p, vector<int> q){

    if(size(p) != size(q)){
        throw invalid_argument( "size of input vectors not equal" );;
    }
    int dist = 0;
    for(int i = 0; i < size(p); i++){
        dist += abs(p[i]-q[i])/(p[i]+q[i]);
    }
    return dist;
};

int zeitreihe::berechneDiskreteFrechetDistanz1D(vector<vector<int>> q, int attribut){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswert1D(q, ca, i, j, attribut);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return d(zeitreihenWerte[i],q[j], attribut);
    }else if(i > 0 && j == 0){
        return max(berechneDiskreteFrechetDistanzFunktionswert1D(q, caIn, i-1, j, attribut),d(zeitreihenWerte[i],q[j], attribut));
    }else if(i == 0 && j > 0){
        return max(berechneDiskreteFrechetDistanzFunktionswert1D(q, caIn, i, j-1, attribut),d(zeitreihenWerte[i],q[j], attribut));
    }else if(i > 0 && j > 0){
        return max(min(min(berechneDiskreteFrechetDistanzFunktionswert1D(q, caIn, i-1, j, attribut),berechneDiskreteFrechetDistanzFunktionswert1D(q, caIn, i-1, j-1, attribut)),
            berechneDiskreteFrechetDistanzFunktionswert1D(q, caIn, i, j-1, attribut)),d(zeitreihenWerte[i],q[j], attribut));
    }else{
        return 1000000;
    }
};

int zeitreihe::berechneDtwDistanz1D(vector<vector<int>> q, int attribut){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDtwDistanzFunktionswert1D(q, ca, i, j, attribut);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

int zeitreihe::berechneDtwDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        return d(zeitreihenWerte[i],q[j], attribut);
    }else if(i > 0 && j == 0){
        return berechneDtwDistanzFunktionswert1D(q, caIn, i-1, j, attribut) + d(zeitreihenWerte[i],q[j], attribut);
    }else if(i == 0 && j > 0){
        return berechneDtwDistanzFunktionswert1D(q, caIn, i, j-1, attribut) + d(zeitreihenWerte[i],q[j], attribut);
    }else if(i > 0 && j > 0){
        return min(min(berechneDtwDistanzFunktionswert1D(q, caIn, i-1, j, attribut),berechneDtwDistanzFunktionswert1D(q, caIn, i-1, j-1, attribut)),
            berechneDtwDistanzFunktionswert1D(q, caIn, i, j-1, attribut)) + d(zeitreihenWerte[i],q[j], attribut);
    }else{
        return 1000000;
    }
};

int zeitreihe::d(vector<int> p, vector<int> q, int attribut){

    return abs(p[attribut] - q[attribut]);
}