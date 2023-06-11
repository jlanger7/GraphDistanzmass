#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "txtFileInterface.h"
#include "knoten.h"
#include "graph.h"
#include "zeitreihe.h"
using namespace std;
using std::filesystem::directory_iterator;

txtFileInterface::txtFileInterface(){

};

vector<string> txtFileInterface::getGraphdatenDateinamen(string ordnerPfad){

    int z = 0;
    for (const auto & file: directory_iterator(ordnerPfad)) { 

        //string dateiPfad(file.path().generic_string());
        string dateiName(file.path().filename().generic_string());
        graphDateiZuordnung.push_back(dateiName);
        cout << "Graph Nr. " + to_string(z) + " represents: " + graphDateiZuordnung[z] << endl;
        z += 1;
    }
    
    return graphDateiZuordnung;
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
        /*if(i%500000==0){
            std::cout << "naechste 500k durchgelaufen i: " + to_string(i) << endl;
            std::cout << "number: " + to_string(number) << endl;
        }*/

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

    int kantenZ = 0;

    int nrTotalVertices = size((*knotenListe));
    for(int v = 0; v < nrTotalVertices-1; v++){

        /*if(v%10000 == 0){
            std::cout << "addEdges, betrachte Knoten nr: " + to_string(v) << endl;
        }*/

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

            /*if(v == 1 && potNeighbor == 2){
                std::cout << "gridIdV: " + to_string(gridId) << endl;
                std::cout << "NrPotNeighbor: " + to_string(potNeighbor) << endl;
                std::cout << "gridIdPotNeighbor: " + to_string(gridIdPotNeighbor) << endl;
                std::cout << "Te: " + to_string(timeStep) + " ex: " + to_string(x) + " ey: " + to_string(y) << endl;
                std::cout << "berID1: " + to_string(calculateGridId(timeStep,x,y+1))  << endl;
                std::cout << "berID2: " + to_string(calculateGridId(timeStep,x+1,y))  << endl;
                }*/
            if(gridIdPotNeighbor == calculateGridId(timeStep,x,y+1) || 
                gridIdPotNeighbor == calculateGridId(timeStep,x+1,y) ||
                    gridIdPotNeighbor == calculateGridId(timeStep+1,x,y)){

                        /*if(v == 1){
                            std::cout << "ADDEN gridIdPotNeighbor: " + to_string(gridIdPotNeighbor) << endl;
                            std::cout << "Te: " + to_string(timeStep) + " ex: " + to_string(x) + " ey: " + to_string(y) << endl;
                            }*/

                        (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                        (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                        kantenZ += 1;
                }
            potNeighbor += 1; 
        }
        delete[] coordinates;
    }
    std::cout << "  Anzahl kanten: " + to_string(kantenZ) << endl;
};

void txtFileInterface::speichereZeitreihe(zeitreihe z, int zeitreihenAttribut, string dateiPfad){

    ofstream myfile(dateiPfad);
    for(int i = 0; i < size(z.getZeitreihenWerte()); i++){

        myfile << z.getZeitreihenWerte()[i][zeitreihenAttribut] << " ";
    }
    myfile.close();
};

void txtFileInterface::speichereCluster(vector<vector<int>> distanzMatrix0, vector<vector<int>> cluster0, vector<vector<int>> distanzMatrix1, vector<vector<int>> cluster1){

    ofstream myfile(pfadOutput + "\\GraphZuordnungUndDistanzmatritzen.txt");

    //schreibe Zuordnung der Graph Nr zu Dateiname weg
    for(int i = 0; i < size(graphDateiZuordnung); i++){

        myfile << "Graph Nr. " + to_string(i) + " = " + graphDateiZuordnung[i] << endl;
    }
    myfile << endl;
    myfile << "Distanzmatrix Attribut 0" << endl;

    //schreibe erste Matrix weg
    for(int i = 0; i < size(distanzMatrix0); i++){
        for(int j = 0; j < size(distanzMatrix0[i]); j++){

            int zahl = distanzMatrix0[i][j];
            int d = 0; 
            if(zahl == 0){
                d = 1;
            }
            while(zahl > 0){
                zahl = zahl/10;
                d += 1;
            }
            int spaceNr = 6 - d;
            myfile << distanzMatrix0[i][j] ;
            for(int d = 0; d < spaceNr; d++){
                myfile << " ";
            }
        }
        myfile << endl;
    }
    myfile << endl;
    myfile <<"Distanzmatrix Attribut 1" << endl;

    //schreibe zweite Matrix weg
    for(int i = 0; i < size(distanzMatrix1); i++){
        for(int j = 0; j < size(distanzMatrix1[i]); j++){

            int zahl = distanzMatrix1[i][j]; 
            int d = 0;
            if(zahl == 0){
                d = 1;
            }
            while(zahl > 0){
                zahl = zahl/10;
                d += 1;
            }
            int spaceNr = 6 - d;
            myfile << distanzMatrix1[i][j] ;
            for(int d = 0; d < spaceNr; d++){
                myfile << " ";
            }
        }
        myfile << endl;
    }
    myfile.close();

    ofstream myfile2(pfadOutput + "\\clusterAttribut0.txt");

    for(int i = 0; i < size(cluster0); i++){
        for(int j = 0; j < size(cluster0[i]); j++){

            myfile2 << cluster0[i][j] << " ";
        }
        myfile2 << "-" << endl;
    }
    myfile2.close();

    ofstream myfile3(pfadOutput + "\\clusterAttribut1.txt");

    for(int i = 0; i < size(cluster1); i++){
        for(int j = 0; j < size(cluster1[i]); j++){

            myfile3 << cluster1[i][j] << " ";
        }
        myfile3 << "-" << endl;
    }
    myfile3.close();
};