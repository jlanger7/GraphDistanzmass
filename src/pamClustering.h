#ifndef pamClustering_H
#define pamClustering_H
#include <vector>
#include <set>
using namespace std;

class pamClustering{

    private:
    vector<vector<int>>* distanzMatrix;
    int k;
    vector<int> medoids;
    vector<int> nichtSelektierteObjekte;


    public:
    pamClustering(vector<vector<int>>* distanzMatrixIn, int kIn);
    void init();
    int getDistanzZumNaechstenGewaehltenMedoid(int objekt);
    int getDistanzZumWeitestenGewaehltenMedoidAusserI(int objekt, int medoidI);
    int getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt);
    vector<int> berechneClustering();


};
#endif