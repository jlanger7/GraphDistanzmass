#ifndef zeitreihe_H
#define zeitreihe__H
#include <vector>
#include <set>
using namespace std;

class zeitreihe{

    private:
    vector<set<int>> teilgraphenKnoten;
    vector<vector<int>> teilgraphenKanten;



    public:
    zeitreihe(vector<int>& knoten, vector<int>& kanten);
    void erstelleTeilgraphenSet(vector<int>& knoten, vector<int>& kanten);


};
#endif