#include "zeitreihe.h"
#include "teilgraphenSet.h"
#include <vector>
#include <iostream>
using namespace std;


zeitreihe::zeitreihe(){

};

zeitreihe::zeitreihe(teilgraphenSet* teilgraphenIn){

    for(int i = 0; i < (*teilgraphenIn).getAnzahlTeilgraphen(); i++){

        vector<double> vecTmp;
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getAnzahlZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMaxVZhk());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getGesVt());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMeanVt());
        vecTmp.push_back((*teilgraphenIn).getTeilgraphen()[i].getMeanNrNachbarn());

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

double zeitreihe::berechneDiskreteFrechetDistanzND(vector<vector<double>> q){

    double** ca = new double*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1.0;

            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswertND(q, ca, i, j);
            /*cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
            cout << " " << endl;*/
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

double zeitreihe::berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j){

    if(caIn[i][j] > -1){
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return max(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j),canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i == 0 && j > 0){
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return max(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i, j-1),canberraDistance(zeitreihenWerte[i],q[j]));
    }else if(i > 0 && j > 0){
        //cout << "fuer ih: " + to_string(i) + " und jot: " + to_string(j) + " nimm:" << endl;
        if(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1))){
            //cout << "   ih: " + to_string(i) + " und jot: " + to_string(j-1) << endl;
        }else if(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1))){
            //cout << "   ih: " + to_string(i-1) + " und jot: " + to_string(j) << endl;
        }else if(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j))){
            //cout << "   ih: " + to_string(i-1) + " und jot: " + to_string(j-1) << endl;
        }
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return max(min(min(berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j),berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i-1, j-1)),
            berechneDiskreteFrechetDistanzFunktionswertND(q, caIn, i, j-1)),canberraDistance(zeitreihenWerte[i],q[j]));
    }else{
        return 1000000;
    }
};

double zeitreihe::berechneDtwDistanzND(vector<vector<double>> q){

    cout << "Groesse zeitreihenwerte: " + to_string(size(zeitreihenWerte)) + " groesse q: " + to_string(size(q)) << endl;
    double** ca = new double*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        cout << "iterarionI: " + to_string(i) << endl;
        ca[i] = new double[size(q)];
        for(int j = 0; j < size(q); j++){
            cout << "was here jot: " + to_string(j) << endl;
            ca[i][j] = -1.0;
            cout << "was here: " + to_string(__LINE__) << endl;
            ca[i][j] = berechneDtwDistanzFunktionswertND(q, ca, i, j);
            cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
            //cout << " " << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

double zeitreihe::berechneDtwDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j){

    cout << "Fktwert ih: " + to_string(i) + " jot: " + to_string(j)  << endl;
    if(caIn[i][j] > -1){
        cout << "Fktwert ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(caIn[i][j]) << endl;
        return caIn[i][j];
    }else if(i == 0 && j == 0){
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j == 0){
        cout << "if" << endl;
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return berechneDtwDistanzFunktionswertND(q, caIn, i-1, j) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i == 0 && j > 0){
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return berechneDtwDistanzFunktionswertND(q, caIn, i, j-1) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else if(i > 0 && j > 0){
        //cout << "fuer ih: " + to_string(i) + " und jot: " + to_string(j) + " nimm:" << endl;
        if(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1))){
            //cout << "   ih: " + to_string(i) + " und jot: " + to_string(j-1) << endl;
        }else if(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1))){
            //cout << "   ih: " + to_string(i-1) + " und jot: " + to_string(j) << endl;
        }else if(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1) <= min(berechneDtwDistanzFunktionswertND(q, caIn, i, j-1),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j))){
            //cout << "   ih: " + to_string(i-1) + " und jot: " + to_string(j-1) << endl;
        }
        //cout << "Berechne canberra fuer ih: " + to_string(i) + " und jot: " + to_string(j)  << endl;
        return min(min(berechneDtwDistanzFunktionswertND(q, caIn, i-1, j),berechneDtwDistanzFunktionswertND(q, caIn, i-1, j-1)),
            berechneDtwDistanzFunktionswertND(q, caIn, i, j-1)) + canberraDistance(zeitreihenWerte[i],q[j]);
    }else{
        return 1000000;
    }
};

double zeitreihe::canberraDistance(vector<double> p, vector<double> q){

    if(size(p) != size(q)){
        throw invalid_argument( "size of input vectors not equal" );;
    }
    double dist = 0;
    for(int i = 0; i < size(p); i++){
        cout << "canberraI: " + to_string(i) << endl;
        dist += (double)abs(p[i]-q[i])/(double)(p[i]+q[i]);
        cout << "canberra: " + to_string(dist) << endl;
    }
    //cout << "canberra: " + to_string(dist) << endl;
    return dist;
};

double zeitreihe::berechneDiskreteFrechetDistanz1D(vector<vector<double>> q, int attribut){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDiskreteFrechetDistanzFunktionswert1D(q, ca, i, j, attribut);
            //cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    return ca[size(zeitreihenWerte)-1][size(q)-1];
};

double zeitreihe::berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<double>> q, int** caIn, int i , int j, int attribut){

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

double zeitreihe::berechneDtwDistanz1D(vector<vector<double>> q, int attribut){

    int** ca = new int*[size(zeitreihenWerte)];
    for(int i = 0; i < size(zeitreihenWerte); i++){
        ca[i] = new int[size(q)];
        for(int j = 0; j < size(q); j++){
            ca[i][j] = -1;

            ca[i][j] = berechneDtwDistanzFunktionswert1D(q, ca, i, j, attribut);
            //cout << "ih: " + to_string(i) + " jot: " + to_string(j) + " ca[i][j]: " + to_string(ca[i][j]) << endl;
        }
    }
    int output = ca[size(zeitreihenWerte)-1][size(q)-1];

    //Speicher deallokieren
    for(int i = 0; i < size(zeitreihenWerte); i++){
        delete [] ca[i];
    }
    delete [] ca;

    return output;
};

double zeitreihe::berechneDtwDistanzFunktionswert1D(vector<vector<double>> q, int** caIn, int i , int j, int attribut){

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

double zeitreihe::d(vector<double> p, vector<double> q, int attribut){

    return abs(p[attribut] - q[attribut]);
}