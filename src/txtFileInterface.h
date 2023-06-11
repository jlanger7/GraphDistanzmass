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
    string pfadOutput = "C:\\Users\\Jonathan Langer\\OneDrive\\Bachelorarbeit\\Experimentdaten";

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
    void speichereCluster(vector<vector<int>> distanzMatrix0, vector<vector<int>> cluster0, vector<vector<int>> distanzMatrix1, vector<vector<int>> cluster1);
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