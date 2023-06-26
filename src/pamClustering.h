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
    double wertKostenFunktion;


    public:
    pamClustering(vector<vector<double>>* distanzMatrixIn, int kIn);
    void init();
    double getDistanzZumNaechstenGewaehltenMedoid(int objekt);
    double getDistanzZumWeitestenGewaehltenMedoidAusserI(int objekt, int medoidI);
    double getDistanzZumZweitNaechstenGewaehltenMedoid(int objekt);
    vector<vector<int>> berechneClustering();
    void berechneZuordnungZuCluster();
    double getWertKostenfunktion(){
        return wertKostenFunktion;
    };
    vector<int>& getMedoids(){
        return medoids;
    };


};
#endif