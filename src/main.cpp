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
   int anzahlBereitsvorhandeneGraphen = 33;

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

   txtFileInterface txt;

   vector<zeitreihe>* zz = txt.einlesenVonZeitreihen(zeitreihenOrdnerPfad);

   vector<vector<int>> distanzMatrixGes1(size(*zz));
   //vector<vector<int>> distanzMatrixMaxZHK(size(graphDateien));
   //vector<vector<int>> distanzMatrixMeanVZHK(size(graphDateien));
   for(int i = 0; i < size(*zz); i++){
      distanzMatrixGes1[i] = vector<int>(size(*zz));
      //distanzMatrixMaxZHK[i] = vector<int>(size(graphDateien));
      //distanzMatrixMeanVZHK[i] = vector<int>(size(graphDateien));
   }

   //Vergleiche jede Zeitreihe...
   for(int i = 0; i < size(*zz); i++){

      cout << "Vergleiche Zeitreihe " + to_string(i) << endl;
      //...mit jeder Zeitreihe
      for(int j = 0; j < size(*zz); j++){

         cout << "   mit Zeitreihe " + to_string(j) << endl;

         //Füge Distanzen den Distanzmatrizen hinzu
         distanzMatrixGes1[i][j] = (*zz)[i].berechneDtwDistanzND((*zz)[j].getZeitreihenWerte());
         //distanzMatrixMaxZHK[i][j] = zeitreihen[i].berechneDtwDistanz1D(zeitreihen[j].getZeitreihenWerte(),1);
         //distanzMatrixMeanVZHK[i][j] = zeitreihen[i].berechneDtwDistanz1D(zeitreihen[j].getZeitreihenWerte(),2);
         //Matrizen sollten symmetrisch sein, also hier evtl auch Werte für [j][i] einfügen und oben zu Beginn pruefen, ob Wert schon
         //berechnet wurde

      }
   }
   delete zz;

   // for(int i = 0; i < size(distanzMatrixGes1); i++){
   //    for(int j = 0; j < size(distanzMatrixGes1[i]); j++){

   //       cout << distanzMatrixGes1[i][j] << "   ";
   //    }
   //    cout << endl;
   // }

   txt.speichereDistanzmatrix(distanzMatrixGes1);

   for(int k = 2; k < 50; k++){

      pamClustering c(&distanzMatrixGes1, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;
      txt.speichereCluster(cluster, k);
   }


   return 0;

   vector<string> graphDateien = txt.getGraphdatenDateinamen(graphDatenOrdnerPfad);
   cout << "Anzahl Graphdateien im Ordner: " + to_string(size(graphDateien)) << endl;
   
   vector<vector<int>> distanzMatrixGes(size(graphDateien));
   //vector<vector<int>> distanzMatrixMaxZHK(size(graphDateien));
   //vector<vector<int>> distanzMatrixMeanVZHK(size(graphDateien));
   for(int i = 0; i < size(graphDateien); i++){
      distanzMatrixGes[i] = vector<int>(size(graphDateien));
      //distanzMatrixMaxZHK[i] = vector<int>(size(graphDateien));
      //distanzMatrixMeanVZHK[i] = vector<int>(size(graphDateien));
   }

   vector<zeitreihe> zeitreihen;

   //Erstelle eine Zeitreihe für jeden Graphen aus dem Inputdaten-Ordner
   for(int i = 0; i < size(graphDateien); i++){

      cout << "Erstelle Zeitreihe fuer Graph " + to_string(i) << endl;
      //----------------Erstelle die erste Zeitreihe-----------------------
      //Erstelle eine Knotenliste aus der Inputdatei für den aktuell betrachteten Graphen, inkl. Zeitschrittattribut für jeden Knoten
      vector<knoten*> knotenListe1 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien[i]);
      //Berechne die Kanten aus der Inputdatei für den aktuell betrachteten Graphen und speichere sie in den Adjazenzlisten der Knoten
      txt.berechneKanten(&knotenListe1);
      //Initialisiere einen Graphen mit der erstellten Knotenliste
      graph* g1 = new graph(knotenListe1);
      cout << "   Graph initialisiert Main" << endl;
      //Initialisiere Teilgraphenset, dass für jeden Zeitschritt im Graphen einen Teilgraphen enthält
      teilgraphenSet* tgSet1 = new teilgraphenSet(g1);
      cout << "   TG-Set initialisiert Main" << endl;
      //Führe die mod. Tiefensuche auf jedem dieser Teilgraphen durch
      for(int z = 0; z < size((*tgSet1).getTeilgraphen()); z++){
         (*tgSet1).getTeilgraphen()[z].modifizierteTiefensuche();
      }
      //Initialisiere eine Zeitreihe für die Teilgraphen
      zeitreihe z1(tgSet1);
      //z1.printZeitreihe();
      txt.speichereZeitreihe(z1, 0, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i) + "_attributNr0_" + graphDateien[i]);
      txt.speichereZeitreihe(z1, 1, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i) + "_attributNr1_" + graphDateien[i]);
      txt.speichereZeitreihe(z1, 2, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(i) + "_attributNr2_" + graphDateien[i]);

      //Füge Zeitreihe dem Zeitreihen-Vektor hinzu fürs spätere vergleichen
      zeitreihen.push_back(z1);

      //Speicher deallokieren
      for(int k = 0; k < size(knotenListe1); k++){
         delete knotenListe1[k];
      }


   }

   //Vergleiche jede Zeitreihe...
   for(int i = 0; i < size(graphDateien); i++){

      cout << "Vergleiche Zeitreihe " + to_string(i) << endl;
      //...mit jeder Zeitreihe
      for(int j = 0; j < size(graphDateien); j++){

         cout << "   mit Zeitreihe " + to_string(j) << endl;

         //Füge Distanzen den Distanzmatrizen hinzu
         distanzMatrixGes[i][j] = zeitreihen[i].berechneDtwDistanzND(zeitreihen[j].getZeitreihenWerte());
         //distanzMatrixMaxZHK[i][j] = zeitreihen[i].berechneDtwDistanz1D(zeitreihen[j].getZeitreihenWerte(),1);
         //distanzMatrixMeanVZHK[i][j] = zeitreihen[i].berechneDtwDistanz1D(zeitreihen[j].getZeitreihenWerte(),2);
         //Matrizen sollten symmetrisch sein, also hier evtl auch Werte für [j][i] einfügen und oben zu Beginn pruefen, ob Wert schon
         //berechnet wurde

      }
   }

   /*cout << "Distanzen Anzahl ZHK" << endl;
   for(int i = 0; i < size(distanzMatrixNrZHK); i++){
      for(int j = 0; j < size(distanzMatrixNrZHK[i]); j++){

         cout << to_string(i) + " zu " + to_string(j) + " = " + to_string(distanzMatrixNrZHK[i][j]) << endl;
      }
   }
   cout << "Distanzen Max. ZHK" << endl;
   for(int i = 0; i < size(distanzMatrixMaxZHK); i++){
      for(int j = 0; j < size(distanzMatrixMaxZHK[i]); j++){

         cout << to_string(i) + " zu " + to_string(j) + " = " + to_string(distanzMatrixMaxZHK[i][j]) << endl;
      }
   }*/
/*
   pamClustering clusteringAttribut0(&distanzMatrixNrZHK, 3);
   vector<vector<int>> cluster0 = clusteringAttribut0.berechneClustering(); 
   pamClustering clusteringAttribut1(&distanzMatrixMaxZHK, 3);
   vector<vector<int>> cluster1 = clusteringAttribut1.berechneClustering();

   txt.speichereCluster(distanzMatrixNrZHK, cluster0, distanzMatrixMaxZHK, cluster1);
*/


   for(int k = 1; k < 10; k++){

      pamClustering c(&distanzMatrixGes, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion());
      //txt.speichereCluster(cluster);
   }

   return 0;

   vector<knoten*> knotenListe1 = txt.readInNodes("C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\20170603._component5_.txt");
   cout << size(knotenListe1) << endl;
   txt.berechneKanten(&knotenListe1);
   graph* g1 = new graph(knotenListe1);
   std::cout << "Zeit Knoten eins: " + to_string((*(*g1).getKnotenMenge()[0]).getZeitAttribut()) << endl;
   teilgraphenSet* tgSet1 = new teilgraphenSet(g1);
   
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   (*tgSet1).erstelleTeilgraphenSet();

   for(int i = 0; i < size((*tgSet1).getTeilgraphen()); i++){

      (*tgSet1).getTeilgraphen()[i].modifizierteTiefensuche();
   }
   
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;

   zeitreihe z1(tgSet1);
   z1.printZeitreihe();



   vector<knoten*> knotenListe2 = txt.readInNodes("C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\20170603._component5_3.txt");
   cout << size(knotenListe2) << endl;
   txt.berechneKanten(&knotenListe2);
   graph* g2 = new graph(knotenListe2);
   teilgraphenSet* tgSet2 = new teilgraphenSet(g2);
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   (*tgSet2).erstelleTeilgraphenSet();
   for(int i = 0; i < size((*tgSet2).getTeilgraphen()); i++){

      (*tgSet2).getTeilgraphen()[i].modifizierteTiefensuche();
   }
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   zeitreihe z2(tgSet2);
   z2.printZeitreihe();

   std::cout << "DTW Attribut #ZHK: " + to_string(z1.berechneDtwDistanz1D(z2.getZeitreihenWerte(),0)) << endl;
   std::cout << "DTW Attribut max V: " + to_string(z1.berechneDtwDistanz1D(z2.getZeitreihenWerte(),1)) << endl;



   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   

   return 0;

  

  
   //pamClustering c(&matrix, 3);
   //vector<vector<int>> cluster = c.berechneClustering();
   /*
   cout << "zurueck in main" << endl;
   for(int k = 0; k < size(cluster); k++){

      cout << "Cluster Nr. " + to_string(k) + " represented durch Medoid " + to_string(c.getMedoids()[k]) + " enthaelt:" << endl;
      for(int i = 0; i < size(cluster[k]); i++){
         cout << "   " + to_string(cluster[k][i]) << endl;
      } 
   }
   cout << "Kostenfunktion: " + to_string(c.getWertKostenfunktion()) << endl;
   */
}





