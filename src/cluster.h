#ifndef cluster_H
#define cluster_H
#include <vector>
#include <set>
using namespace std;

class cluster{

    private:
    vector<vector<int>>* distanzMatrix;


    public:
    cluster(vector<vector<int>>* distanzMatrixIn);
    vector<int> initPAM(int k);
    int getDistanzZumNaechstenGewaehltenMedoid(vector<int> initMedoids, int objekt);


};
#endif