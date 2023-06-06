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

int main(){

   txtFileInterface txt;



   vector<knoten*> knotenListe1 = txt.readInNodes("C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\20170603._component0.txt");
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



   vector<knoten*> knotenListe2 = txt.readInNodes("C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\20170603._component5.txt");
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

   std::cout << "Diskrete Frechet Distanz: " + to_string(z1.berechneDiskreteFrechetDistanzND(z2.getZeitreihenWerte())) << endl;



   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   

   return 0;
}





