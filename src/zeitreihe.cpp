#include "zeitreihe.h"
#include "teilgraphenSet.h"
#include <vector>
#include <iostream>
using namespace std;


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