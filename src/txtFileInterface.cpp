#include <vector>
#include <fstream>
#include <iostream>
#include "txtFileInterface.h"
#include "knoten.h"
using namespace std;

txtFileInterface::txtFileInterface(){

};

int txtFileInterface::getTimeStep(int gridId){

    if(gridId >= (dimTime*dimX*dimY)){
        gridId -= (dimTime*dimX*dimY);
    }

    int timeStep = 0;
    while(gridId - (dimX*dimY) >= 0){
        timeStep += 1;
        gridId -= (dimX*dimY);
    }
    return timeStep;
};

int* txtFileInterface::calculateCoordinates(int gridId){

    if(gridId >= (dimTime*dimX*dimY)){
        gridId -= (dimTime*dimX*dimY);
    }

    int* coordinates = new int[3];

    int timeStep = 0;
    while(gridId - (dimX*dimY) >= 0){
        timeStep += 1;
        gridId -= (dimX*dimY);
    }
    coordinates[0] = timeStep;

    int x = 0;
    while(gridId - dimY >= 0){
        x += 1;
        gridId -= dimY;
    }
    coordinates[1] = x;

    int y = gridId;
    coordinates[2] = y;

    return coordinates;

}

vector<knoten*> txtFileInterface::readInNodes(string dateiPfad){

    ifstream file(dateiPfad);
    int number;
    bool nrIsDbzValue = false;
    int dbz, gridId;
    int i = 0;
    int offset = 0;
    vector<knoten*> knotenListe;

    while(file >> number) {
        i += 1;

        //Logging zum Test
        if(i%500000==0){
            std::cout << "naechste 500k durchgelaufen i: " + to_string(i) << endl;
            std::cout << "number: " + to_string(number) << endl;
        }

        //Jede zweite Zahl in der Datei ist nicht gebrauchter DBZ-Wert, nur GridId relevant hier
        if(nrIsDbzValue){

            nrIsDbzValue = false;

        }else{

            gridId = number;
            int timeStep = getTimeStep(gridId);
            nrIsDbzValue = true;
            //if(i == 1){
            //offset = gridId;
            //}
        
        gridId = gridId - offset;
        knoten* v = new knoten(gridId, timeStep);
        knotenListe.push_back(v);
        }
    }
   
   file.close();
   return knotenListe;
};
