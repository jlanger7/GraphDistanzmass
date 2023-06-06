#include <vector>
#include <fstream>
#include <iostream>
#include "txtFileInterface.h"
#include "knoten.h"
#include "graph.h"
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
    int timeStepOffset = 0;
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
            if(i == 1){
            //offset = gridId;
            timeStepOffset = getTimeStep(gridId) - 1;
            }
        
        gridId = gridId - offset;
        knoten* v = new knoten(gridId, timeStep-timeStepOffset);
        knotenListe.push_back(v);
        }
    }
   
   file.close();
   return knotenListe;
};

void txtFileInterface::berechneKanten(vector<knoten*>* knotenListe){

    int nrTotalVertices = size((*knotenListe));
    for(int v = 0; v < nrTotalVertices-1; v++){

        if(v%10000 == 0){
            std::cout << "addEdges, betrachte Knoten nr: " + to_string(v) << endl;
        }

        int gridId = (*(*knotenListe)[v]).getId();
        //std::cout << "was here: " + to_string(__LINE__) << endl;

        int* coordinates = calculateCoordinates(gridId);
        int timeStep = *(coordinates + 0);
        int x = *(coordinates + 1);
        int y = *(coordinates + 2);

        int potNeighbor = v + 1;
        int gridIdPotNeighbor = (*(*knotenListe)[potNeighbor]).getId();
       
        while((gridIdPotNeighbor - gridId) <= ((dimX*dimY)+dimX) && potNeighbor < nrTotalVertices){

            gridIdPotNeighbor = (*(*knotenListe)[potNeighbor]).getId();
            if(gridIdPotNeighbor == calculateGridId(timeStep,x,y+1) || 
                gridIdPotNeighbor == calculateGridId(timeStep,x+1,y) ||
                    gridIdPotNeighbor == calculateGridId(timeStep+1,x,y)){

                        if(v == 0){
                            std::cout << "gridIdPotNeighbor: " + to_string(gridIdPotNeighbor) << endl;
                            std::cout << "Te: " + to_string(timeStep) + " ex: " + to_string(x) + " ey: " + to_string(y) << endl;
                            }

                        (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                        (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                }
            potNeighbor += 1; 
        }
        delete[] coordinates;
    }
};
