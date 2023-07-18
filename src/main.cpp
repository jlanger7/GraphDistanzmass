#include <vector>
#include <fstream>
#include <iostream>
#include <netcdf>
#include <math.h>
#include <iomanip>
#include <regex>
#include <ctime>
#include <filesystem>
#include <chrono>
#include "txtFileInterface.h"
#include "knoten.h"
#include "graph.h"
#include "teilgraphenSet.h"
#include "zeitreihe.h"
#include "pamClustering.h"
using namespace std;

string graphDatenOrdnerPfad = "data/GraphDateien";
string zeitreihenOrdnerPfad = "data/zeitreihen";

void graphenZuZeitreihenTransformieren(){

   txtFileInterface txt;
   //Graphdateien einlesen
   vector<string> graphDateien1 = txt.getGraphdatenDateinamen(graphDatenOrdnerPfad);
   cout << "Anzahl Graphdateien im Ordner: " + to_string(size(graphDateien1)) << endl;
   int anzahlBereitsvorhandeneGraphen = 0;

   //Erstelle eine Zeitreihe für jeden Graphen aus dem Inputdaten-Ordner
   int graphNr = 0;
   cout << "Anzahl Dateien " + to_string(size(graphDateien1)) << endl;
   for(int i = 0; i < size(graphDateien1); i++){

      auto now = std::chrono::system_clock::now();
      std::time_t t_c = std::chrono::system_clock::to_time_t(now);
      std::cout << "Startzeit neue Methode " << std::ctime(&t_c);
      //----------------Erstelle die Zeitreihe-----------------------
      //Erstelle eine Knotenliste aus der Inputdatei für den aktuell betrachteten Graphen, inkl. Zeitschrittattribut für jeden Knoten
      vector<knoten*> knotenListe1 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien1[i]);
      if((*knotenListe1[0]).getZeitAttribut() != 0 && (*knotenListe1[size(knotenListe1)-1]).getZeitAttribut() != 287){

         //Berechne die Kanten aus der Inputdatei für den aktuell betrachteten Graphen und speichere sie in den Adjazenzlisten der Knoten
         txt.berechneKanten(&knotenListe1);
         //Initialisiere einen Graphen mit der erstellten Knotenliste
         graph* g1 = new graph(knotenListe1);
         cout << "   Graph initialisiert Main" << endl;
         //Initialisiere Teilgraphenset, dass für jeden Zeitschritt im Graphen einen Teilgraphen enthält
         teilgraphenSet* tgSet1 = new teilgraphenSet(g1);
         cout << "   TG-Set initialisiert Main" << endl;
         //Führe die mod. Tiefensuche auf jedem dieser Teilgraphen durch
         cout << "   Anzahl TG: " + to_string(size((*tgSet1).getTeilgraphen())) << endl;
         for(int z = 0; z < size((*tgSet1).getTeilgraphen()); z++){
            cout << "   mod.Tiefensuche fuer TG Nr: " + to_string(z) << endl;
            (*tgSet1).getTeilgraphen()[z].modifizierteTiefensucheNeu();
         }
         //Initialisiere eine Zeitreihe für die Teilgraphen
         zeitreihe z1(tgSet1);
         //z1.printZeitreihe();
         //Speichere Zeitreihen einzeln pro Zeitreihenattribut
         txt.speichereZeitreihe(z1, 0, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(graphNr) + "_attributNr0_" + graphDateien1[i]);
         txt.speichereZeitreihe(z1, 1, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(graphNr) + "_attributNr1_" + graphDateien1[i]);
         txt.speichereZeitreihe(z1, 2, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(graphNr) + "_attributNr2_" + graphDateien1[i]);
         graphNr += 1;
      }else{

      }
      //Speicher deallokieren
      for(int k = 0; k < size(knotenListe1); k++){
         delete knotenListe1[k];
      }
      auto now2 = std::chrono::system_clock::now();
      std::time_t t_c2 = std::chrono::system_clock::to_time_t(now2);
      std::cout << "Endzeit neue Methode " << std::ctime(&t_c2);
   }
}

void clusteringBerechnen(int startk, int endK, string distanzmass, vector<vector<double>> distanzMatrixGes){

   txtFileInterface txt;
   
   vector<double> wertKostenfunktionGes;
   //Berechne Clustering für alle k von "startK" bis "endK" und speichere Clustereinteilungen der Objekte
   for(int k = startk; k < endK+1; k++){

      //Clustering auf Distanzmatrix, die mit DTW oder Frechet Distanz berechnet wurde
      pamClustering c(&distanzMatrixGes, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;
      wertKostenfunktionGes.push_back(c.getWertKostenfunktion());
      txt.speichereCluster(cluster, k, "AttributeGesamt_"+distanzmass, c.getWertKostenfunktion());
   }

   txt.speichereWerteKostenfunktion(wertKostenfunktionGes, "Gesamt_"+distanzmass);
}

void zeitreihenDistanzmatritzenUndClusteringBerechnen(bool dtw){

   txtFileInterface txt;
   vector<zeitreihe>* zz = txt.einlesenVonZeitreihen(zeitreihenOrdnerPfad, 3);

   cout << "Groesse zz: " + to_string(size(*zz)) << endl;

   vector<vector<double>> distanzMatrixGes(size(*zz));
   for(int i = 0; i < size(*zz); i++){
      distanzMatrixGes[i] = vector<double>(size(*zz));
      
      for(int j = 0; j < size(*zz); j++){
         distanzMatrixGes[i][j] = -1;
      }
   }

   string verwendetesDistanzmass;
   //Vergleiche jede Zeitreihe...
   for(int i = 0; i < size(*zz); i++){

      cout << "Vergleiche Zeitreihe " + to_string(i) << endl;
      //...mit jeder Zeitreihe
      for(int j = 0; j < size(*zz); j++){

         cout << "   mit Zeitreihe " + to_string(j) << endl;

         //Distanzen sind symmetrisch
         if(distanzMatrixGes[i][j] == -1 || distanzMatrixGes[j][i] == -1){

            //Füge Distanzen den Distanzmatrizen hinzu 
            if(!dtw){
               distanzMatrixGes[i][j] = (*zz)[i].berechneDiskreteFrechetDistanzND((*zz)[j].getZeitreihenWerte());
               distanzMatrixGes[j][i] = distanzMatrixGes[i][j];
               verwendetesDistanzmass = "frechet";
            }else if(dtw){
               distanzMatrixGes[i][j] = (*zz)[i].berechneDtwDistanzND((*zz)[j].getZeitreihenWerte());
               distanzMatrixGes[j][i] = distanzMatrixGes[i][j];
               verwendetesDistanzmass = "dtw";
            }
         }
      }
   }
   txt.speichereDistanzmatrix(distanzMatrixGes, "AttributeGesamt_"+verwendetesDistanzmass);

   clusteringBerechnen(2, 5, verwendetesDistanzmass, distanzMatrixGes);
}

int main(){

   graphenZuZeitreihenTransformieren();

   zeitreihenDistanzmatritzenUndClusteringBerechnen(true);
   //zeitreihenDistanzmatritzenUndClusteringBerechnen(false);

   //Clustering berechnen für Distanzmatrizen, die bereits berechnet und abgespeichert worden sind
   // txtFileInterface txt;
   // vector<vector<double>> distanzMatrixGes = txt.einlesenVonDistanzmatrix("AttributeGesamt_dtw", 100);
   // clusteringBerechnen(2, 5, "dtw", distanzMatrixGes);

   return 0;

}