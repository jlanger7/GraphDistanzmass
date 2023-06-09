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
#include "../tests/zeitreiheTest.h"
using namespace std;

string graphDatenOrdnerPfad = "C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\calculateDistance";

int main(){

   txtFileInterface txt;

   vector<string> graphDateien = txt.getGraphdatenDateinamen(graphDatenOrdnerPfad);
   cout << "Anzahl Graphdateien im Ordner: " + to_string(size(graphDateien)) << endl;
   
   int distanzMatrixNrZHK[size(graphDateien)][size(graphDateien)];
   int distanzMatrixMaxZHK[size(graphDateien)][size(graphDateien)];

   //Vergleiche jeden Graphen aus dem Inputdaten-Ordner...
   for(int i = 0; i < size(graphDateien); i++){
      //...mit jedem Graphen aus dem Inputdaten-Ordner
      for(int j = 0; j < size(graphDateien); j++){

         //----------------Erstelle die erste Zeitreihe-----------------------
         //Erstelle eine Knotenliste aus der Inputdatei für den aktuell betrachteten Graphen, inkl. Zeitschrittattribut für jeden Knoten
         vector<knoten*> knotenListe1 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien[i]);
         //Berechne die Kanten aus der Inputdatei für den aktuell betrachteten Graphen und speichere sie in den Adjazenzlisten der Knoten
         txt.berechneKanten(&knotenListe1);
         //Initialisiere einen Graphen mit der erstellten Knotenliste
         graph* g1 = new graph(knotenListe1);
         //Initialisiere Teilgraphenset, dass für jeden Zeitschritt im Graphen einen Teilgraphen enthält
         teilgraphenSet* tgSet1 = new teilgraphenSet(g1);
         //Führe die mod. Tiefensuche auf jedem dieser Teilgraphen durch
         for(int z = 0; z < size((*tgSet1).getTeilgraphen()); z++){
            (*tgSet1).getTeilgraphen()[z].modifizierteTiefensuche();
         }
         //Initialisiere eine Zeitreihe für die Teilgraphen
         zeitreihe z1(tgSet1);
         z1.printZeitreihe();

         //----------------Erstelle die zweite Zeitreihe-----------------------
         //Erstelle eine Knotenliste aus der Inputdatei für den aktuell zu vergleichenden Graphen, inkl. Zeitschrittattribut für jeden 
         //Knoten
         vector<knoten*> knotenListe2 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien[j]);
         //Berechne die Kanten aus der Inputdatei für den aktuell betrachteten Graphen und speichere sie in den Adjazenzlisten der Knoten
         txt.berechneKanten(&knotenListe2);
         //Initialisiere einen Graphen mit der erstellten Knotenliste
         graph* g2 = new graph(knotenListe2);
         //Initialisiere Teilgraphenset, dass für jeden Zeitschritt im Graphen einen Teilgraphen enthält
         teilgraphenSet* tgSet2 = new teilgraphenSet(g2);
         //Führe die mod. Tiefensuche auf jedem dieser Teilgraphen durch
         for(int z = 0; z < size((*tgSet2).getTeilgraphen()); z++){
            (*tgSet2).getTeilgraphen()[z].modifizierteTiefensuche();
         }
         //Initialisiere eine Zeitreihe für die Teilgraphen
         zeitreihe z2(tgSet2);
         z2.printZeitreihe();

         distanzMatrixNrZHK[i][j] = z1.berechneDtwDistanz1D(z2.getZeitreihenWerte(),0);
         distanzMatrixMaxZHK[i][j] = z1.berechneDtwDistanz1D(z2.getZeitreihenWerte(),1);
         //Matrizen sollten symmetrisch sein, also hier evtl auch Werte für [j][i] einfügen und oben zu Beginn pruefen, ob Wert schon
         //berechnet wurde

      }
   }
   cout << "Distanzen Anzahl ZHK" << endl;
   for(int i = 0; i < sizeof(distanzMatrixNrZHK); i++){
      for(int j = 0; j < sizeof(distanzMatrixNrZHK[i]); j++){

         cout << to_string(i) + " zu " + to_string(j) + " = " + to_string(distanzMatrixNrZHK[i][j]) << endl;
      }
   }
   cout << "Distanzen Max. ZHK" << endl;
   for(int i = 0; i < sizeof(distanzMatrixMaxZHK); i++){
      for(int j = 0; j < sizeof(distanzMatrixMaxZHK[i]); j++){

         cout << to_string(i) + " zu " + to_string(j) + " = " + to_string(distanzMatrixMaxZHK[i][j]) << endl;
      }
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
}





