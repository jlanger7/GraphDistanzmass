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
    const static int anzahlAttribute = 5;
    void printZeitreihe();
    double berechneDiskreteFrechetDistanzND(vector<vector<double>> q);
    double berechneDiskreteFrechetDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j);
    double berechneDtwDistanzND(vector<vector<double>> q);
    double berechneDtwDistanzFunktionswertND(vector<vector<double>> q, double** caIn, int i , int j);
    double canberraDistance(vector<double> p, vector<double> q);
    double berechneDiskreteFrechetDistanz1D(vector<vector<double>> q, int attribut);
    double berechneDiskreteFrechetDistanzFunktionswert1D(vector<vector<double>> q, double** caIn, int i , int j, int attribut);
    double berechneDtwDistanz1D(vector<vector<double>> q, int attribut);
    double berechneDtwDistanzFunktionswert1D(vector<vector<double>> q, double** caIn, int i , int j, int attribut);
    double d(vector<double> p, vector<double> q, int attribut);
    void setZeitreihenWerte(vector<vector<double>> in){
        zeitreihenWerte = in;
    };
    vector<vector<double>> getZeitreihenWerte(){
        return zeitreihenWerte;
    };
        

};
#endif