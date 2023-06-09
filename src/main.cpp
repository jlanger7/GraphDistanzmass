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

string graphDatenOrdnerPfad = "C:\\Users\\Jonathan Langer\\OneDrive\\Bachelorarbeit\\Experimentdaten\\100GraphenGleichmClustering\\GraphDateienToProcessGleichmCluster";
string zeitreihenOrdnerPfad = "C:\\Users\\Jonathan Langer\\OneDrive\\Bachelorarbeit\\Experimentdaten\\100GraphenGleichmClustering\\zeitreihen";

void graphenZuZeitreihenTransformieren(){

   int fehlendeGraphen[] = {0,9,16,22,31,35,39,40,50,51,92};
   txtFileInterface txt;
   //---------------Neue Methode Kanten berechnen Testen-----------------
   vector<string> graphDateien1 = txt.getGraphdatenDateinamen(graphDatenOrdnerPfad);
   cout << "Anzahl Graphdateien im Ordner: " + to_string(size(graphDateien1)) << endl;
   int anzahlBereitsvorhandeneGraphen = 0;

   //Erstelle eine Zeitreihe für jeden Graphen aus dem Inputdaten-Ordner
   int graphNr = 0;
   cout << "Anzahl Dateien " + to_string(size(graphDateien1)) << endl;
   for(int i = 0; i < size(graphDateien1); i++){

      cout << "Erstelle Zeitreihe fuer Graph " + to_string(fehlendeGraphen[i]) << endl;
      auto now = std::chrono::system_clock::now();
      std::time_t t_c = std::chrono::system_clock::to_time_t(now);
      std::cout << "Startzeit neue Methode " << std::ctime(&t_c);
      //----------------Erstelle die erste Zeitreihe-----------------------
      //Erstelle eine Knotenliste aus der Inputdatei für den aktuell betrachteten Graphen, inkl. Zeitschrittattribut für jeden Knoten
      vector<knoten*> knotenListe1 = txt.readInNodes(graphDatenOrdnerPfad + "\\" + graphDateien1[i]);
      if((*knotenListe1[0]).getZeitAttribut() != 0 && (*knotenListe1[size(knotenListe1)-1]).getZeitAttribut() != 287){

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
         txt.speichereZeitreihe(z1, 0, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(fehlendeGraphen[i]) + "_attributNr0_" + graphDateien1[i]);
         txt.speichereZeitreihe(z1, 1, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(fehlendeGraphen[i]) + "_attributNr1_" + graphDateien1[i]);
         txt.speichereZeitreihe(z1, 2, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(fehlendeGraphen[i]) + "_attributNr2_" + graphDateien1[i]);
         //txt.speichereZeitreihe(z1, 3, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(graphNr+anzahlBereitsvorhandeneGraphen) + "_attributNr3_" + graphDateien1[i]);
         //txt.speichereZeitreihe(z1, 4, zeitreihenOrdnerPfad+"\\" + "graphNr" + to_string(graphNr+anzahlBereitsvorhandeneGraphen) + "_attributNr4_" + graphDateien1[i]);
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

void clusteringBerechnen(int startk, int endK, string distanzmass, vector<vector<double>> distanzMatrixGes, vector<vector<double>> distanzMatrixNrZHK,vector<vector<double>> distanzMatrixMaxZHK ,vector<vector<double>> distanzMatrixGesVt){

   txtFileInterface txt;
   
   vector<double> wertKostenfunktionGes;
   vector<double> wertKostenfunktion0;
   vector<double> wertKostenfunktion1;
   vector<double> wertKostenfunktion2;
   //vector<double> wertKostenfunktion3;
   //vector<double> wertKostenfunktion4;
   for(int k = startk; k < endK+1; k++){

      pamClustering c(&distanzMatrixGes, k);
      vector<vector<int>> cluster = c.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c.getWertKostenfunktion()) << endl;
      wertKostenfunktionGes.push_back(c.getWertKostenfunktion());
      txt.speichereCluster(cluster, k, "AttributeGesamt_"+distanzmass, c.getWertKostenfunktion());

      pamClustering c1(&distanzMatrixNrZHK, k);
      vector<vector<int>> cluster1 = c1.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c1.getWertKostenfunktion()) << endl;
      wertKostenfunktion0.push_back(c1.getWertKostenfunktion());
      txt.speichereCluster(cluster1, k, "Attribut0_"+distanzmass, c1.getWertKostenfunktion());

      pamClustering c2(&distanzMatrixMaxZHK, k);
      vector<vector<int>> cluster2 = c2.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c2.getWertKostenfunktion()) << endl;
      wertKostenfunktion1.push_back(c2.getWertKostenfunktion());
      txt.speichereCluster(cluster2, k, "Attribut1_"+distanzmass, c2.getWertKostenfunktion());

      pamClustering c3(&distanzMatrixGesVt, k);
      vector<vector<int>> cluster3 = c3.berechneClustering();
      cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c3.getWertKostenfunktion()) << endl;
      wertKostenfunktion2.push_back(c3.getWertKostenfunktion());
      txt.speichereCluster(cluster3, k, "Attribut2_"+distanzmass, c3.getWertKostenfunktion());

      // pamClustering c4(&distanzMatrixMeanNrNachbar, k);
      // vector<vector<int>> cluster4 = c4.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c4.getWertKostenfunktion()) << endl;
      // wertKostenfunktion3.push_back(c4.getWertKostenfunktion());
      // txt.speichereCluster(cluster4, k, "Attribut4_"+distanzmass, c4.getWertKostenfunktion());

      // pamClustering c5(&distanzMatrixMeanNrNachbar, k);
      // vector<vector<int>> cluster5 = c5.berechneClustering();
      // cout << "Kosten mit k = " + to_string(k) + " : " + to_string(c5.getWertKostenfunktion()) << endl;
      // wertKostenfunktion4.push_back(c5.getWertKostenfunktion());
      // txt.speichereCluster(cluster5, k, "Attribut4_"+distanzmass, c5.getWertKostenfunktion());
   }

   txt.speichereWerteKostenfunktion(wertKostenfunktionGes, "Gesamt_"+distanzmass);
   txt.speichereWerteKostenfunktion(wertKostenfunktion0, "Attribut0_"+distanzmass);
   txt.speichereWerteKostenfunktion(wertKostenfunktion1, "Attribut1_"+distanzmass);
   txt.speichereWerteKostenfunktion(wertKostenfunktion2, "Attribut2_"+distanzmass);
   //txt.speichereWerteKostenfunktion(wertKostenfunktion3, "Attribut3_"+distanzmass);
   //txt.speichereWerteKostenfunktion(wertKostenfunktion4, "Attribut4_"+distanzmass);
}

void zeitreihenDistanzmatritzenBerechnen(bool dtw){

   txtFileInterface txt;
   vector<zeitreihe>* zz = txt.einlesenVonZeitreihen(zeitreihenOrdnerPfad);

   cout << "Groesse zz: " + to_string(size(*zz)) << endl;

   vector<vector<double>> distanzMatrixGes(size(*zz));
   vector<vector<double>> distanzMatrixNrZHK(size(*zz));
   vector<vector<double>> distanzMatrixMaxZHK(size(*zz));
   vector<vector<double>> distanzMatrixGesVt(size(*zz));
   //vector<vector<double>> distanzMatrixMeanVt(size(*zz));
   vector<vector<double>> distanzMatrixMeanNrNachbarn(size(*zz));
   for(int i = 0; i < size(*zz); i++){
      distanzMatrixGes[i] = vector<double>(size(*zz));
      distanzMatrixNrZHK[i] = vector<double>(size(*zz));
      distanzMatrixMaxZHK[i] = vector<double>(size(*zz));
      distanzMatrixGesVt[i] = vector<double>(size(*zz));
      //distanzMatrixMeanVt[i] = vector<double>(size(*zz));
      distanzMatrixMeanNrNachbarn[i] = vector<double>(size(*zz));
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
            distanzMatrixNrZHK[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),0);
            distanzMatrixMaxZHK[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),1);
            distanzMatrixGesVt[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),2);
            //distanzMatrixMeanVt[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),3);
            //distanzMatrixMeanNrNachbarn[i][j] = (*zz)[i].berechneDiskreteFrechetDistanz1D((*zz)[j].getZeitreihenWerte(),4);
            verwendetesDistanzmass = "frechet";
         }else if(dtw){
            distanzMatrixGes[i][j] = (*zz)[i].berechneDtwDistanzND((*zz)[j].getZeitreihenWerte());
            distanzMatrixNrZHK[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),0);
            distanzMatrixMaxZHK[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),1);
            distanzMatrixGesVt[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),2);
            //distanzMatrixMeanVt[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),3);
            //distanzMatrixMeanNrNachbarn[i][j] = (*zz)[i].berechneDtwDistanz1D((*zz)[j].getZeitreihenWerte(),4);
            verwendetesDistanzmass = "dtw";
         }
         //Matrizen sollten symmetrisch sein, also hier evtl auch Werte für [j][i] einfügen und oben zu Beginn pruefen, ob Wert schon
         //berechnet wurde
      }
   }
   txt.speichereDistanzmatrix(distanzMatrixGes, "AttributeGesamt_"+verwendetesDistanzmass);
   txt.speichereDistanzmatrix(distanzMatrixNrZHK, "Attribut0_"+verwendetesDistanzmass);
   txt.speichereDistanzmatrix(distanzMatrixMaxZHK, "Attribut1_"+verwendetesDistanzmass);
   txt.speichereDistanzmatrix(distanzMatrixGesVt, "Attribut2_"+verwendetesDistanzmass);
   //txt.speichereDistanzmatrix(distanzMatrixMeanVt, "Attribut3_"+verwendetesDistanzmass);
   //txt.speichereDistanzmatrix(distanzMatrixMeanNrNachbarn, "Attribut4_"+verwendetesDistanzmass);

   clusteringBerechnen(2, 10, verwendetesDistanzmass, distanzMatrixGes, distanzMatrixNrZHK, distanzMatrixMaxZHK, distanzMatrixGesVt);
}

void clusterTest(){

   vector<vector<double>> matrix{
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

   zeitreihenDistanzmatritzenBerechnen(true);
   //zeitreihenDistanzmatritzenBerechnen(false);

   // txtFileInterface txt;

   // vector<vector<double>> distanzMatrixGes = txt.einlesenVonDistanzmatrix("AttributeGesamt_dtw", 100);
   // vector<vector<double>> distanzMatrixNrZHK = txt.einlesenVonDistanzmatrix("Attribut0_dtw", 100);
   // vector<vector<double>> distanzMatrixMaxZHK = txt.einlesenVonDistanzmatrix("Attribut1_dtw", 100);;
   // vector<vector<double>> distanzMatrixGesVZHK = txt.einlesenVonDistanzmatrix("Attribut2_dtw", 100);;

   // clusteringBerechnen(2, 50, "dtw", distanzMatrixGes, distanzMatrixNrZHK, distanzMatrixMaxZHK, distanzMatrixGesVZHK);
   
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





