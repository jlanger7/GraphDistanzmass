#ifndef zeitreiheTest_H
#define zeitreiheTest_H
#include <vector>
#include <set>
using namespace std;

class zeitreiheTest{

    private:
    vector<int> knoten;
    vector<int> kanten;



    public:
    zeitreiheTest();
    void setInputGraphen();
    bool testeErstelleTeilgraphenSet();

    void printTeilgraphKnotenUndKanten();


};
#endif