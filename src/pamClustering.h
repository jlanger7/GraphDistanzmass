#ifndef pamClustering_H
#define pamClustering_H
#include <vector>
#include <set>
using namespace std;

class pamClustering{

    private:
    vector<vector<double>>* distanzMatrix;
    int k;
    vector<int> medoids;
    vector<int> nichtSelektierteObjekte;
    vector<vector<int>> zuordnungZuCluster;
    int wertKostenFunktion;


    public:
    pamClustering(vector<vector<double>>* distanzMatrixIn, int kIn);
    void init();
    int getDistanzZumNaechstenGewaehltenMedoid(int objekt);
    int getDistanzZumWeitestenGewaehltenMedoidAusserI(int objekt, int medoidI);
    int getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt);
    vector<vector<int>> berechneClustering();
    void berechneZuordnungZuCluster();
    int getWertKostenfunktion(){
        return wertKostenFunktion;
    };
    vector<int>& getMedoids(){
        return medoids;
    };


};
#endif