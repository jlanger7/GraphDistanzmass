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
        

};
#endif