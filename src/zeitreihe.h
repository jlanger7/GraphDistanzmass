#ifndef zeitreihe_H
#define zeitreihe_H
#include <vector>
#include "teilgraphenSet.h"
using namespace std;

class zeitreihe{

    private:
    vector<vector<int>> zeitreihenWerte;



    public:
    zeitreihe();
    zeitreihe(teilgraphenSet* teilgraphenIn);
    void printZeitreihe();
    int berechneDiskreteFrechetDistanzND(vector<vector<int>> q);
    int berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<int>> q, int** caIn, int i , int j);
    int berechneDtwDistanzND(vector<vector<int>> q);
    int berechneDtwDistanzFunktionswertND(vector<vector<int>> q, int** caIn, int i , int j);
    int canberraDistance(vector<int> p, vector<int> q);
    int berechneDiskreteFrechetDistanz1D(vector<vector<int>> q, int attribut);
    int berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut);
    int berechneDtwDistanz1D(vector<vector<int>> q, int attribut);
    int berechneDtwDistanzFunktionswert1D(vector<vector<int>> q, int** caIn, int i , int j, int attribut);
    int d(vector<int> p, vector<int> q, int attribut);
    void setZeitreihenWerte(vector<vector<int>> in){
        zeitreihenWerte = in;
    };
    vector<vector<int>> getZeitreihenWerte(){
        return zeitreihenWerte;
    };
        

};
#endif