#ifndef pamClustering_H
#define pamClustering_H
#include <vector>
#include <set>
using namespace std;

class pamClustering{

    private:
    vector<vector<int>>* distanzMatrix;
    vector<int> medoids;
    vector<int> nichtSelektierteObjekte;


    public:
    pamClustering(vector<vector<int>>* distanzMatrixIn);
    vector<int> init(int k);
    int getDistanzZumNaechstenGewaehltenMedoid( int objekt);
    int getDistanzZumWeitestenGewaehltenMedoidAusserI(int objekt, int medoidI);
    int getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt);
    vector<int> clustering();


};
#endif