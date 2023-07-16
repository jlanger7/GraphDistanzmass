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

        string dateiName(file.path().filename().generic_string());
        graphDateiZuordnung.push_back(dateiName);
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

//Berechne die Koordinaten der Knoten aus ihrer ID in den Textdateien => KnotenID = (y + (x*dimY) + (t*dimX*dimY))
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
    int timeStepOffset = 0;
    vector<knoten*> knotenListe;

    //Hilfsarray, um Existenz potentieller Nachbarknoten beim Hinzufügen der Kanten zu prüfen
    hilfsArrayKanten = new vector<int>*[dimTime];
    for(int t = 0; t < dimTime; t++){
        hilfsArrayKanten[t] = new vector<int>[dimX];
        for(int x = 0; x < dimX; x++){
            for(int y = 0; y < dimY; y++){
                hilfsArrayKanten[t][x].push_back(-1); 
            }
        }
    } 

    while(file >> number) {
        i += 1;

        //Jede zweite Zahl in der Datei ist nicht gebrauchter DBZ-Wert, nur GridId relevant hier
        if(nrIsDbzValue){

            nrIsDbzValue = false;

        }else{
            nrIsDbzValue = true;
            gridId = number;

            int* coordinates = calculateCoordinates(gridId);
            int timeStep = *(coordinates + 0);
            int x = *(coordinates + 1);
            int y = *(coordinates + 2);
            
            //Offset für den Zeitschritt, damit die Zeitschritte in den Graphen immer bei 1 starten
            if(i == 1){
            //offset = gridId;
            timeStepOffset = getTimeStep(gridId) - 1;
            }

            //Erstelle Knoten und füge ihn der Knotenliste und dem Hilfsarray für die spätere Kantenerstellung hinzu
            knoten* v = new knoten(gridId, timeStep-timeStepOffset);
            knotenListe.push_back(v);
            hilfsArrayKanten[timeStep][x][y] = size(knotenListe)-1;

            delete[] coordinates;
        }
    }
   
   file.close();
   anzahlKnoten.push_back(size(knotenListe));
   return knotenListe;
};

void txtFileInterface::berechneKanten(vector<knoten*>* knotenListe){

    int kantenZ = 0;
    int nrTotalVertices = size((*knotenListe));

    //Erstelle die Kanten für jeden Knoten
    for(int v = 0; v < nrTotalVertices-1; v++){

        //Berechne die Koordinaten des aktuellen Knotens, um später auf die potentiellen Nachbarn zu prüfen
        int gridId = (*(*knotenListe)[v]).getId();
        int* coordinates = calculateCoordinates(gridId);
        int timeStep = *(coordinates + 0);
        int x = *(coordinates + 1);
        int y = *(coordinates + 2);

        //Prüfe, ob die potentiellen Nachbarn im Hilfsarray für die Kantenerstellung vorhanden sind und füge sie zu den Adajazenzlisten beider Knoten hinzu, 
        //falls dies der Fall ist
        if((y+1) < dimY){
            int potNeighbor = hilfsArrayKanten[timeStep][x][y+1];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        if((x+1) < dimX){
            int potNeighbor = hilfsArrayKanten[timeStep][x+1][y];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        if((timeStep+1) < dimTime){
            int potNeighbor = hilfsArrayKanten[timeStep+1][x][y];
            if(potNeighbor >= 0){
                (*(*knotenListe)[v]).addNachbarn((*knotenListe)[potNeighbor]);
                (*(*knotenListe)[potNeighbor]).addNachbarn((*knotenListe)[v]);
                kantenZ += 1;
            }
        }
        //Deallokieren des Speichers
        delete[] coordinates;
    }
    
    //Deallokieren des Speichers
    for(int t = 0; t < dimTime; t++){
        delete[] hilfsArrayKanten[t];
    }
    delete[] hilfsArrayKanten;
};

void txtFileInterface::speichereZeitreihe(zeitreihe z, int zeitreihenAttribut, string dateiPfad){

    ofstream myfile(dateiPfad);
    //Liste nach und nach alle Werte der eindimensionalen Zeitreihe über das vorgegebene Attribut auf, getrennt durch Leerzeichen
    for(int i = 0; i < size(z.getZeitreihenWerte()); i++){

        myfile << z.getZeitreihenWerte()[i][zeitreihenAttribut] << " ";
    }
    myfile.close();
};

vector<zeitreihe>* txtFileInterface::einlesenVonZeitreihen(string ordnerPfad, int anzahlZeitreihenAttribute){

    //Lese alle Dateinamen im Zeitreihenordner ein
    int c = 0;
    vector<string> zeitreihenDateien;
    for (const auto & file: directory_iterator(ordnerPfad)) { 

        zeitreihenDateien.push_back(file.path().generic_string());
        c += 1;
    }

    //Pro Graph sind "anzahlZeitreihenAttribute"-viele Zeitreihen vorhanden, eine für jedes Zeitreihenattribut
    vector<zeitreihe>* zeitreihen = new vector<zeitreihe>;
    //Für jeden Graphen,...
    for(int i = 0; i < c/anzahlZeitreihenAttribute; i++){

        zeitreihe z;
        vector<vector<double>> zeitreihenWerte;

        //...lese die "anzahlZeitreihenAttribute"-vielen Zeitreihen ein
        for(int j = 0; j < anzahlZeitreihenAttribute; j++){
            for(int k = 0; k < size(zeitreihenDateien); k++){
                
                //Finde die richtige Datei für das aktuell betrachtete Zeitreihenattribut j
                if(zeitreihenDateien[k].find("graphNr"+to_string(i)+"_") != std::string::npos && zeitreihenDateien[k].find("attributNr"+to_string(j)+"_") != std::string::npos){

                    ifstream file(zeitreihenDateien[k]);
                    double number;
                    int nrElement = 0;
                    while(file >> number) {
                        
                        //Wenn aktuell das erste Zeitreihenattribut betrachtet wird, dann erstelle für jeden Beobachtungspunkt ein Zeitreihenelement und füge den Wert hinzu
                        if(j == 0){
                            
                            vector<double> zeitreihenElement;
                            zeitreihenElement.push_back(number);
                            zeitreihenWerte.push_back(zeitreihenElement);
                        }
                        //Wird ein anderes, als das erste Zeitreihenattribut betrachtet, dann hänge den Wert an das entsprechende Zeitreihenelement 
                        else{

                            zeitreihenWerte[nrElement].push_back(number);
                        }
                        nrElement += 1;
                    }
                }
            }
        }
        //Füge die Zeitreihenwerte dem Zeitreihenobjekt hinzu
        z.setZeitreihenWerte(zeitreihenWerte);
        (*zeitreihen).push_back(z);
    }

    return zeitreihen;    
};

void txtFileInterface::speichereDistanzmatrix(vector<vector<int>> distanzMatrix, string distanzMatrixBezeichner){

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

            ifstream file(dateien[d]);
            double number;
            int i = 0;
            int j = 0;
            while(file >> number) {
                
                

                distanzMatrix[i][j] = number;
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

    myfile2.close();
}

void txtFileInterface::speichereWerteKostenfunktion(vector<double> werteKostenfunktion, string bezeichner){

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");

    ofstream myfile2(pfadOutput + "\\" + "WerteKostenfunktion_Cluster_"+ bezeichner + "_" + datetime.str() + ".txt");

    for(int i = 0; i < size(werteKostenfunktion); i++){
        myfile2 << werteKostenfunktion[i] << " ";
    }

    myfile2.close();
}