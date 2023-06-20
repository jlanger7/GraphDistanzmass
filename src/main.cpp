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
#include "../tests/zeitreiheTest.h"
using namespace std;

string graphDatenOrdnerPfad = "C:\\Users\\Jonathan Langer\\OneDrive\\Bachelorarbeit\\Experimentdaten\\comparingProcessedData\\calculateDistance";
string zeitreihenOrdnerPfad = "C:\\Users\\Jonathan Langer\\OneDrive\\Bachelorarbeit\\Experimentdaten\\zeitreihen";

void graphenZuZeitreihenTransformieren(){

   txtFileInterface txt;
   //---------------Neue Methode Kanten berechnen Testen-----------------
   vector<string> graphDateien1 = txt.getGraphdatenDateinamen(graphDatenOrdnerPfad);
   cout << "Anzahl Graphdateien im Ordner: " + to_string(size(graphDateien1)) << endl;
   int anzahlBereitsvorhandeneGraphen = 0;

   //Erstelle eine Zeitreihe für jeden Graphen aus dem Inputdaten-Ordner
   for(int i = 0; i < size(graphDateien1); i++){

      cout << "Erstelle Zeitreihe fuer Graph " + to_string(i) << endl;
      auto now = std::chrono::system_clock::now();
      std::time_t t_c = std::chrono::system_clock::to_time_t(now);
      std::cout << "Startzeit neue Methode " << std::ctime(&t_c);
      //----------------Erstelle die erste Zeitreihe-----------------------
      //Erstelle eine Knotenliste aus der Inputdatei für den aktuell betrachteten Graphen, inkl. Zeitschrittattribut für jeden Knoten
      vector<knoten*> knotenListe1 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien1[i]);
      //Berechne die Kanten aus der Inputdatei für den aktuell betrachteten Graphen und speichere sie in den Adjazenzlisten der Knoten
      txt.berechneKantenTest(&knotenListe1);
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
      txt.speichereZeitreihe(z1, 0, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i+anzahlBereitsvorhandeneGraphen) + "_attributNr0_" + graphDateien1[i]);
      txt.speichereZeitreihe(z1, 1, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i+anzahlBereitsvorhandeneGraphen) + "_attributNr1_" + graphDateien1[i]);
      txt.speichereZeitreihe(z1, 2, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i+anzahlBereitsvorhandeneGraphen) + "_attributNr2_" + graphDateien1[i]);

      //Speicher deallokieren
      for(int k = 0; k < size(knotenListe1); k++){
         delete knotenListe1[k];
      }
      auto now2 = std::chrono::system_clock::now();
      std::time_t t_c2 = std::chrono::system_clock::to_time_t(now2);
      std::cout << "Endzeit neue Methode " << std::ctime(&t_c2);
   }
}

void clusteringBerechnen(int startk, int endK, string distanzmass, vector<vector<float>> distanzMatrixGes, vector<vector<int>> distanzMatrixNrZHK,vector<vector<int>> distanzMatrixMaxZHK ,vector<vector<int>> distanzMatrixMeanVZHK){

   txtFileInterface txt;
   
   vector<int> wertKostenfunktionGes;
   vector<int> wertKostenfunktion0;
   vector<int> wertKostenfunktion1;
   vector<int> wertKostenfunktion2;
   for(int k = startk; k < endK+1; k++){

      pamClustering c(&distanzMatrixGes, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;
      wertKostenfunktionGes.push_back(c.getWertKostenfunktion());
      txt.speichereCluster(cluster, k, "AttributeGesamt_"+distanzmass, c.getWertKostenfunktion());

      // pamClustering c1(&distanzMatrixNrZHK, k);
      // vector<vector<int>> cluster1 = c1.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c1.getWertKostenfunktion()) << endl;
      // wertKostenfunktion0.push_back(c1.getWertKostenfunktion());
      // txt.speichereCluster(cluster1, k, "Attribut0_"+distanzmass, c1.getWertKostenfunktion());

      // pamClustering c2(&distanzMatrixMaxZHK, k);
      // vector<vector<int>> cluster2 = c2.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c2.getWertKostenfunktion()) << endl;
      // wertKostenfunktion1.push_back(c2.getWertKostenfunktion());
      // txt.speichereCluster(cluster2, k, "Attribut1_"+distanzmass, c2.getWertKostenfunktion());

      // pamClustering c3(&distanzMatrixMeanVZHK, k);
      // vector<vector<int>> cluster3 = c3.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c3.getWertKostenfunktion()) << endl;
      // wertKostenfunktion2.push_back(c3.getWertKostenfunktion());
      // txt.speichereCluster(cluster3, k, "Attribut2_"+distanzmass, c3.getWertKostenfunktion());
   }

   txt.speichereWerteKostenfunktion(wertKostenfunktionGes, "Gesamt_"+distanzmass);
   // txt.speichereWerteKostenfunktion(wertKostenfunktion0, "Attribut0_"+distanzmass);
   // txt.speichereWerteKostenfunktion(wertKostenfunktion1, "Attribut1_"+distanzmass);
   // txt.speichereWerteKostenfunktion(wertKostenfunktion2, "Attribut2_"+distanzmass);
}

void zeitreihenDistanzmatritzenBerechnen(bool dtw){

   txtFileInterface txt;
   vector<zeitreihe>* zz = txt.einlesenVonZeitreihen(zeitreihenOrdnerPfad);

   vector<vector<float>> distanzMatrixGes(size(*zz));
   vector<vector<int>> distanzMatrixNrZHK(size(*zz));
   vector<vector<int>> distanzMatrixMaxZHK(size(*zz));
   vector<vector<int>> distanzMatrixMeanVZHK(size(*zz));
   for(int i = 0; i < size(*zz); i++){
      distanzMatrixGes[i] = vector<float>(size(*zz));
      distanzMatrixNrZHK[i] = vector<int>(size(*zz));
      distanzMatrixMaxZHK[i] = vector<int>(size(*zz));
      distanzMatrixMeanVZHK[i] = vector<int>(size(*zz));
   }

   string verwendetesDistanzmass;
   //Vergleiche jede Zeitreihe...
   for(int i = 0; i < size(*zz); i++){

      cout << "Vergleiche Zeitreihe " + to_string(i) << endl;
      //...mit jeder Zeitreihe
      for(int j = 0; j < size(*zz); j++){

         cout << "   mit Zeitreihe " + to_string(j) << endl;

         //Füge Distanzen den Distanzmatrizen hinzu
         if(!dtw){
            distanzMatrixGes[i][j] = (*zz)[i].berechneDiskreteFrechetDistanzND((*zz)[j].getZeitreihenWerte());
            // distanzMatrixNrZHK[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),0);
            // distanzMatrixMaxZHK[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),1);
            // distanzMatrixMeanVZHK[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),2);
            verwendetesDistanzmass = "frechet";
         }else if(dtw){
            distanzMatrixGes[i][j] = (*zz)[i].berechneDtwDistanzND((*zz)[j].getZeitreihenWerte());
            distanzMatrixNrZHK[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),0);
            distanzMatrixMaxZHK[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),1);
            distanzMatrixMeanVZHK[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),2);
            verwendetesDistanzmass = "dtw";
         }
         //Matrizen sollten symmetrisch sein, also hier evtl auch Werte für [j][i] einfügen und oben zu Beginn pruefen, ob Wert schon
         //berechnet wurde
      }
   }
   txt.speichereDistanzmatrix(distanzMatrixGes, "AttributeGesamt_"+verwendetesDistanzmass);
   // txt.speichereDistanzmatrix(distanzMatrixNrZHK, "Attribut0_"+verwendetesDistanzmass);
   // txt.speichereDistanzmatrix(distanzMatrixMaxZHK, "Attribut1_"+verwendetesDistanzmass);
   // txt.speichereDistanzmatrix(distanzMatrixMeanVZHK, "Attribut2_"+verwendetesDistanzmass);

   clusteringBerechnen(2, 50, verwendetesDistanzmass, distanzMatrixGes, distanzMatrixNrZHK, distanzMatrixMaxZHK, distanzMatrixMeanVZHK);
}

void clusterTest(){

   vector<vector<int>> matrix{
      {0,1,5,5,5,5},
      {1,0,5,5,5,5},
      {5,5,0,1,5,5},
      {5,5,1,0,5,5},
      {5,5,5,5,0,1},
      {5,5,5,5,1,0}
   };

   for(int k = 1; k < 20; k++){

      pamClustering c(&matrix, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;

      for(int j = 0; j < size(cluster); j++){
      cout << "Cluster Nr. " + to_string(j) + " represented durch Medoid " + to_string(c.getMedoids()[j]) + " enthaelt:" << endl;
         for(int i = 0; i < size(cluster[j]); i++){
            cout << "   " + to_string(cluster[j][i]) << endl;
         } 
      }
      //txt.speichereCluster(distanzMatrixGes1, cluster);
   }

};

int main(){

   //graphenZuZeitreihenTransformieren();

   zeitreihenDistanzmatritzenBerechnen(false);

   //txtFileInterface txt;

   //vector<vector<int>> distanzMatrixGes = txt.einlesenVonDistanzmatrix("AttributeGesamt_frechet", 194);
   // vector<vector<int>> distanzMatrixNrZHK = txt.einlesenVonDistanzmatrix("Attribut0_frechet", 194);;
   // vector<vector<int>> distanzMatrixMaxZHK = txt.einlesenVonDistanzmatrix("Attribut1_frechet", 194);;
   // vector<vector<int>> distanzMatrixMeanVZHK = txt.einlesenVonDistanzmatrix("Attribut2_frechet", 194);;

   // clusteringBerechnen(2, 50, "frechet", distanzMatrixGes, distanzMatrixNrZHK, distanzMatrixMaxZHK, distanzMatrixMeanVZHK);
   
   return 0;



   
   
   for(int k = 26; k < 51; k++){

      // pamClustering c(&distanzMatrixGes1, k);
      // vector<vector<int>> cluster = c.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;
      // wertKostenfunktionGes.push_back(c.getWertKostenfunktion());
      // txt.speichereCluster(cluster, k, "AttributeGesamt", c.getWertKostenfunktion());

      /*pamClustering c1(&distanzMatrixNrZHK, k);
      vector<vector<int>> cluster1 = c1.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c1.getWertKostenfunktion()) << endl;
      wertKostenfunktion0.push_back(c1.getWertKostenfunktion());
      txt.speichereCluster(cluster1, k, "Attribut0", c1.getWertKostenfunktion());

      pamClustering c2(&distanzMatrixMaxZHK, k);
      vector<vector<int>> cluster2 = c2.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c2.getWertKostenfunktion()) << endl;
      wertKostenfunktion1.push_back(c2.getWertKostenfunktion());
      txt.speichereCluster(cluster2, k, "Attribut1", c2.getWertKostenfunktion());*/

      // pamClustering c3(&distanzMatrixMeanVZHK, k);
      // vector<vector<int>> cluster3 = c3.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c3.getWertKostenfunktion()) << endl;
      // wertKostenfunktion2.push_back(c3.getWertKostenfunktion());
      // txt.speichereCluster(cluster3, k, "Attribut2", c3.getWertKostenfunktion());
   }

   //txt.speichereWerteKostenfunktion(wertKostenfunktionGes, "Gesamt");
   //txt.speichereWerteKostenfunktion(wertKostenfunktion0, "Attribut0");
   //txt.speichereWerteKostenfunktion(wertKostenfunktion1, "Attribut1");
   //txt.speichereWerteKostenfunktion(wertKostenfunktion2, "Attribut2");


   return 0;

}





