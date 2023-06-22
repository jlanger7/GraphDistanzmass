#ifndef zeitreihe_H
#define zeitreihe_H
#include <vector>
#include "teilgraphenSet.h"
using namespace std;

class zeitreihe{

    private:
    vector<vector<double>> zeitreihenWerte;



    public:
    zeitreihe();
    zeitreihe(teilgraphenSet* teilgraphenIn);
    void printZeitreihe();
    float berechneDiskreteFrechetDistanzND(vector<vector<int>> q);
    float berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<int>> q, float** caIn, int i , int j);
    float berechneDtwDistanzND(vector<vector<int>> q);
    float berechneDtwDistanzFunktionswertND(vector<vector<int>> q, float** caIn, int i , int j);
    float canberraDistance(vector<int> p, vector<int> q);
    float berechneDiskreteFrechetDistanz1D(vector<vector<int>> q, int attribut);
    float berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut);
    float berechneDtwDistanz1D(vector<vector<int>> q, int attribut);
    float berechneDtwDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut);
    float d(vector<int> p, vector<int> q, int attribut);
    void setZeitreihenWerte(vector<vector<double>> in){
        zeitreihenWerte = in;
    };
    vector<vector<double>> getZeitreihenWerte(){
        return zeitreihenWerte;
    };
        

};
#endif