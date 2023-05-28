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
    int berechneDiskreteFrechetDistanz(vector<vector<int>> q);
    int berechneDiskreteFrechetDistanzFunktionswert(vector<vector<int>> q, int** caIn, int i , int j);
    int berechneDtwDistanz(vector<vector<int>> q);
    int berechneDtwDistanzFunktionswert(vector<vector<int>> q, int** caIn, int i , int j);
    int d(vector<int> p, vector<int> q);
    void setZeitreihenWerte(vector<vector<int>> in){
        zeitreihenWerte = in;
    };
    vector<vector<int>> getZeitreihenWerte(){
        return zeitreihenWerte;
    };
        

};
#endif