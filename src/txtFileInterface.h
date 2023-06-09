#ifndef txtFileInterface_H
#define txtFileInterface_H
#include <vector>
#include <string>
#include "knoten.h"
#include "graph.h"
#include "zeitreihe.h"

class txtFileInterface{

    private:
    const int dimTime = 288;
    const int dimX = 1501;
    const int dimY = 1501;
    vector<string> graphDateiZuordnung;
    vector<int> anzahlKnoten;
    string pfadOutput = " ";
    vector<int>** hilfsArrayKanten;

    public:
    txtFileInterface();
    int getTimeStep(int gridId);
    int* calculateCoordinates(int gridId);
    std::vector<knoten*> readInNodes(std::string dateiPfad);
    void berechneKanten(vector<knoten*>* knotenListe);
    vector<string> getGraphdatenDateinamen(string ordnerPfad);
    int calculateGridId(int t, int x, int y){
        return (y + (x*dimY) + (t*dimX*dimY));
    }
    void speichereZeitreihe(zeitreihe z, int zeitreihenAttribut, string dateiPfad);
    vector<zeitreihe>* einlesenVonZeitreihen(string ordnerPfad, int anzahlZeitreihenAttribute);
    void speichereDistanzmatrix(vector<vector<int>> distanzMatrix, string distanzMatrixBezeichner);
    void speichereDistanzmatrix(vector<vector<double>> distanzMatrix, string distanzMatrixBezeichner);
    vector<vector<double>> einlesenVonDistanzmatrix(string matrixBezeichner, int dimension);
    void speichereCluster(vector<vector<int>> cluster, int k, string clusterBezeichner, int wertKostenfunktion);
    void speichereWerteKostenfunktion(vector<double> werteKostenfunktion, string bezeichner);
    void graphZuordnungUndAnzahlKnoten();

    int getDimTime(){
        return dimTime;
    };
    int getDimX(){
        return dimX;
    };
    int getDimY(){
        return dimY;
    };

};
#endif