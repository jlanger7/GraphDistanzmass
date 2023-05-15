#ifndef zeitreihe_H
#define zeitreihe_H
#include <vector>
#include <set>
using namespace std;

class zeitreihe{

    private:
    vector<set<int>> teilgraphenKnoten;
    vector<vector<vector<int>>> teilgraphenKanten;
    int anzahlTeilgraphen;



    public:
    zeitreihe(vector<int>& knoten, vector<int>& kanten);
    void erstelleTeilgraphenSet(vector<int>& knoten, vector<int>& kanten);
    int* modifizierteTiefensuche(int teilgrpahNr);
    

    void printTeilgraphKnotenUndKanten();
    vector<set<int>>& getTeilgraphenKnoten(){
        return teilgraphenKnoten;
    };
    vector<vector<vector<int>>>& getTeilgraphenKanten(){
        return teilgraphenKanten;
    };


};
#endif