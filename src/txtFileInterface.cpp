#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>  
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

    //Opt Versuch
    helpArr = new vector<int>*[dimTime];
    for(int t = 0; t < dimTime; t++){
        helpArr[t] = new vector<int>[dimX];
        for(int x = 0; x < dimX; x++){
            for(int y = 0; y < dimY; y++){
                helpArr[t][x].push_back(-1); 
            }
        }
    } 

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

            int* coordinates = calculateCoordinates(gridId);
            int timeStep = *(coordinates + 0);
            int x = *(coordinates + 1);
            int y = *(coordinates + 2);

            nrIsDbzValue = true;
            if(i == 1){
            //offset = gridId;
            timeStepOffset = getTimeStep(gridId) - 1;
            }
        
            gridId = gridId - offset;
            knoten* v = new knoten(gridId, timeStep-timeStepOffset);
            knotenListe.push_back(v);
            helpArr[timeStep][x][y] = size(knotenListe)-1;

            delete[] coordinates;
        }
    }
   
   file.close();
   cout << "Knoten eingelesen" << endl;
   anzahlKnoten.push_back(size(knotenListe));
   return knotenListe;
};

void txtFileInterface::berechneKanten(vector<knoten*>* knotenListe){

    int kantenZ = 0;

    int nrTotalVertices = size((*knotenListe));

    cout << "Anzahl Knoten: " + to_string(nrTotalVertices) << endl;

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

        int potGridId1 = calculateGridId(timeStep,x,y+1);
        int potGridId2 = calculateGridId(timeStep,x+1,y);
        int potGridId3 = calculateGridId(timeStep+1,x,y);
        int potNeighbor = v + 1;

        //if(v%100000 == 0){
        //    cout << "   Pruefe Kanten fuer Knoten: " + to_string(v) + " gridID: " + to_string(gridId) << endl;
        //}

        int gridIdPotNeighbor = (*(*knotenListe)[potNeighbor]).getId();
       
        while((gridIdPotNeighbor - gridId) <= ((dimX*dimY)+dimX) && potNeighbor < nrTotalVertices){

            gridIdPotNeighbor = (*(*knotenListe)[potNeighbor]).getId();

            if(potNeighbor%50000 == 0 && v%100000 == 0){
                cout << "   Pruefe pot Nachbar: " + to_string(potNeighbor) + " gridID: " + to_string(gridIdPotNeighbor) + " fuer vau: " +to_string(v) + " gridID: " + to_string(gridId) << endl;
            }

            /*if(v == 1 && potNeighbor == 2){
                std::cout << "gridIdV: " + to_string(gridId) << endl;
                std::cout << "NrPotNeighbor: " + to_string(potNeighbor) << endl;
                std::cout << "gridIdPotNeighbor: " + to_string(gridIdPotNeighbor) << endl;
                std::cout << "Te: " + to_string(timeStep) + " ex: " + to_string(x) + " ey: " + to_string(y) << endl;
                std::cout << "berID1: " + to_string(calculateGridId(timeStep,x,y+1))  << endl;
                std::cout << "berID2: " + to_string(calculateGridId(timeStep,x+1,y))  << endl;
                }*/
            if(gridIdPotNeighbor == potGridId1 || 
                gridIdPotNeighbor == potGridId2 ||
                    gridIdPotNeighbor == potGridId3){

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

void txtFileInterface::berechneKantenTest(vector<knoten*>* knotenListe){

    int kantenZ = 0;
    int nrTotalVertices = size((*knotenListe));
    cout << "Anzahl Knoten: " + to_string(nrTotalVertices) << endl;

    for(int v = 0; v < nrTotalVertices-1; v++){

        if(v%100000 == 0){
            std::cout << "addEdges, betrachte Knoten nr: " + to_string(v) << endl;
        }

        int gridId = (*(*knotenListe)[v]).getId();
        //std::cout << "was here: " + to_string(__LINE__) << endl;

        int* coordinates = calculateCoordinates(gridId);
        int timeStep = *(coordinates + 0);
        int x = *(coordinates + 1);
        int y = *(coordinates + 2);

        if((y+1) < dimY){
            int potNeighbor = helpArr[timeStep][x][y+1];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        if((x+1) < dimX){
            int potNeighbor = helpArr[timeStep][x+1][y];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        if((timeStep+1) < dimTime){
            int potNeighbor = helpArr[timeStep+1][x][y];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        delete[] coordinates;
    }
    
    for(int t = 0; t < dimTime; t++){
        delete[] helpArr[t];
    }
    delete[] helpArr;
    std::cout << "  Anzahl kanten: " + to_string(kantenZ) << endl;
};

void txtFileInterface::speichereZeitreihe(zeitreihe z, int zeitreihenAttribut, string dateiPfad){

    ofstream myfile(dateiPfad);
    for(int i = 0; i < size(z.getZeitreihenWerte()); i++){

        myfile << z.getZeitreihenWerte()[i][zeitreihenAttribut] << " ";
    }
    myfile.close();
};

vector<zeitreihe>* txtFileInterface::einlesenVonZeitreihen(string ordnerPfad){

    int c = 0;
    vector<string> zeitreihenDateien;
    for (const auto & file: directory_iterator(ordnerPfad)) { 

        //string dateiPfad(file.path().generic_string());
        zeitreihenDateien.push_back(file.path().generic_string());
        c += 1;
    }

    vector<zeitreihe>* zeitreihen = new vector<zeitreihe>;
    int anzahlZeitreihenAttribute = zeitreihe::anzahlAttribute;
    cout << "Anzahl Graphen: " + to_string(c/anzahlZeitreihenAttribute) << endl;
    for(int i = 0; i < c/anzahlZeitreihenAttribute; i++){

        zeitreihe z;
        vector<vector<double>> zeitreihenWerte;
        for(int j = 0; j < anzahlZeitreihenAttribute; j++){
            for(int k = 0; k < size(zeitreihenDateien); k++){
                
                if(zeitreihenDateien[k].find("graphNr"+to_string(i)+"_") != std::string::npos && zeitreihenDateien[k].find("attributNr"+to_string(j)+"_") != std::string::npos){

                    ifstream file(zeitreihenDateien[k]);
                    cout << "lese ein " + zeitreihenDateien[k] << endl;
                    double number;
                    int nrElement = 0;
                    while(file >> number) {
                        
                        if(j == 0){
                            
                            vector<double> zeitreihenElement;
                            zeitreihenElement.push_back(number);
                            zeitreihenWerte.push_back(zeitreihenElement);
                        }else{

                            zeitreihenWerte[nrElement].push_back(number);
                        }

                        nrElement += 1;
                    }
                }
            }
        }
        z.setZeitreihenWerte(zeitreihenWerte);
        (*zeitreihen).push_back(z);
    }

    return zeitreihen;    
};

void txtFileInterface::speichereDistanzmatrix(vector<vector<int>> distanzMatrix, string distanzMatrixBezeichner){

    cout << "speichere Distanzmatrix" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    ofstream myfile(pfadOutput + "\\Distanzmatrix_" + distanzMatrixBezeichner + "_" + datetime.str() + ".txt");

    //schreibe Distanzmatrix weg
    for(int i = 0; i < size(distanzMatrix); i++){
        for(int j = 0; j < size(distanzMatrix[i]); j++){

            int zahl = distanzMatrix[i][j];
            int d = 0; 
            if(zahl == 0){
                d = 1;
            }
            while(zahl > 0){
                zahl = zahl/10;
                d += 1;
            }
            int spaceNr = 10 - d;
            //cout << "speichere Wert: " + to_string(distanzMatrix[i][j]) << endl;
            myfile << distanzMatrix[i][j] ;
            for(int d = 0; d < spaceNr; d++){
                myfile << " ";
            }
        }
        myfile << endl;
    }
    myfile << endl;
    myfile.close();
};

void txtFileInterface::speichereDistanzmatrix(vector<vector<double>> distanzMatrix, string distanzMatrixBezeichner){

    cout << "speichere Distanzmatrix" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    ofstream myfile(pfadOutput + "\\Distanzmatrix_" + distanzMatrixBezeichner + "_" + datetime.str() + ".txt");

    //schreibe Distanzmatrix weg
    for(int i = 0; i < size(distanzMatrix); i++){
        for(int j = 0; j < size(distanzMatrix[i]); j++){

            int zahl = distanzMatrix[i][j];
            int d = 0; 
            if(zahl == 0){
                d = 1;
            }
            while(zahl > 0){
                zahl = zahl/10;
                d += 1;
            }
            int spaceNr = 15 - d;
            cout << "speichere Wert: " + to_string(distanzMatrix[i][j]) << endl;
            myfile << distanzMatrix[i][j] ;
            for(int d = 0; d < spaceNr; d++){
                myfile << " ";
            }
        }
        myfile << endl;
    }
    myfile << endl;
    myfile.close();
};

vector<vector<double>> txtFileInterface::einlesenVonDistanzmatrix(string matrixBezeichner, int dimension){

    vector<string> dateien;
    for (const auto & file: directory_iterator(pfadOutput)) { 

        //string dateiPfad(file.path().generic_string());
        dateien.push_back(file.path().generic_string());
    }

    vector<vector<double>> distanzMatrix(dimension);
    for(int i = 0; i < dimension; i++){
        distanzMatrix[i] = vector<double>(dimension);
    }
    for(int d = 0; d < size(dateien); d++){

        if(dateien[d].find("Distanzmatrix") != std::string::npos && dateien[d].find(matrixBezeichner) != std::string::npos){

            cout << "datei gefunden: " + dateien[d] << endl;

            ifstream file(dateien[d]);
            double number;
            int i = 0;
            int j = 0;
            while(file >> number) {
                
                

                distanzMatrix[i][j] = number;
                cout << "nr " + to_string(number) << endl;
                if(number == 0){
                cout << "Distanz 0 in Spalte: " + to_string(j) << endl;    
                }
                j += 1;
                if (j == dimension){
                    i += 1;
                    j = 0;
                }
            }
        }
    }
    return distanzMatrix;
};

void txtFileInterface::speichereCluster(vector<vector<int>> cluster, int k, string clusterBezeichner, int wertKostenfunktion){

    cout << "   speichere Cluster" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    ofstream myfile2(pfadOutput + "\\k_" + to_string(k) + "_cluster_"+ clusterBezeichner + "_" + datetime.str() + ".txt");

    for(int i = 0; i < size(cluster); i++){
        for(int j = 0; j < size(cluster[i]); j++){

            myfile2 << cluster[i][j] << " ";
        }
        myfile2 << "-" << endl;
    }
    //myfile2 << "Wert Kostenfunktion: " + to_string(wertKostenfunktion) << endl; 

    myfile2.close();
}

void txtFileInterface::speichereWerteKostenfunktion(vector<double> werteKostenfunktion, string bezeichner){

    cout << "   speichere WerteKostenfunktion" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    ofstream myfile2(pfadOutput + "\\" + "WerteKostenfunktion_Cluster_"+ bezeichner + "_" + datetime.str() + ".txt");

    for(int i = 0; i < size(werteKostenfunktion); i++){
        myfile2 << werteKostenfunktion[i] << " ";
    }
    //myfile2 << "Wert Kostenfunktion: " + to_string(wertKostenfunktion) << endl; 

    myfile2.close();
}

void txtFileInterface::speichereCluster(vector<vector<int>> distanzMatrix0, vector<vector<int>> cluster0, vector<vector<int>> distanzMatrix1, vector<vector<int>> cluster1){

    cout << "speichere Cluster" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M");

    cout << "datetime berechnet" << endl;
    
    ofstream myfile(pfadOutput + "\\GraphZuordnungUndDistanzmatritzen_" + datetime.str() + ".txt");

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
            int spaceNr = 10 - d;
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
            int spaceNr = 10 - d;
            myfile << distanzMatrix1[i][j] ;
            for(int d = 0; d < spaceNr; d++){
                myfile << " ";
            }
        }
        myfile << endl;
    }
    myfile.close();

    ofstream myfile2(pfadOutput + "\\clusterAttribut0_" + datetime.str() + ".txt");

    for(int i = 0; i < size(cluster0); i++){
        for(int j = 0; j < size(cluster0[i]); j++){

            myfile2 << cluster0[i][j] << " ";
        }
        myfile2 << "-" << endl;
    }
    myfile2.close();

    ofstream myfile3(pfadOutput + "\\clusterAttribut1_" + datetime.str() + ".txt");

    for(int i = 0; i < size(cluster1); i++){
        for(int j = 0; j < size(cluster1[i]); j++){

            myfile3 << cluster1[i][j] << " ";
        }
        myfile3 << "-" << endl;
    }
    myfile3.close();
};

void txtFileInterface::graphZuordnungUndAnzahlKnoten(){

    cout << "speichere graphZuordnungUndAnzahlKnoten" << endl;

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    cout << "datetime berechnet" << endl;
    
    ofstream myfile(pfadOutput + "\\GraphZuordnungUndAnzahlKnoten_" + datetime.str() + ".txt");

    //schreibe Zuordnung der Graph Nr zu Dateiname weg
    for(int i = 0; i < size(graphDateiZuordnung); i++){

        myfile << "Graph Nr. " + to_string(i) + " = " + graphDateiZuordnung[i] + " - Anzahl Knoten: " + to_string(anzahlKnoten[i]) << endl;
    }
    myfile << endl;

    cout << "Done speichere graphZuordnungUndAnzahlKnoten" << endl;
};