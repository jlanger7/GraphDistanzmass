#ifndef txtFileInterface_H
#define txtFileInterface_H
#include <vector>
#include <string>
#include "knoten.h"

class txtFileInterface{

    private:
    const int dimTime = 288;
    const int dimX = 1501;
    const int dimY = 1501;

    public:
    txtFileInterface();
    int getTimeStep(int gridId);
    int* calculateCoordinates(int gridId);
    std::vector<knoten*> readInNodes(std::string dateiPfad);

    int calculateGridId(int t, int x, int y){
        return (y + (x*dimY) + (t*dimX*dimY));
    }
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