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

   vector<knoten*> knotenListe1 = txt.readInNodes("C:\\Users\\Jonathan Langer\\Desktop\\SHK Job\\VS Projects\\data\\comparingProcessedData\\20170603._component5.txt");
   cout << size(knotenListe1) << endl;
   txt.berechneKanten(&knotenListe1);
   graph* g1 = new graph(knotenListe1);
   teilgraphenSet* tgSet1 = new teilgraphenSet(g1);
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   (*tgSet1).erstelleTeilgraphenSet();
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   for(int i = 0; i < (*tgSet1).getAnzahlTeilgraphen(); i++){

      (*tgSet1).getTeilgraphen()[i].modifizierteTiefensuche();
   }
   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   cout << "Anzahl zhk erster teilgraph: " + to_string((*tgSet1).getTeilgraphen()[0].getAnzahlZhk()) << endl;
   cout << "Anzahl zhk erster min: " + to_string((*tgSet1).getTeilgraphen()[0].getMinVZhk()) << endl;
   cout << "Anzahl zhk erster max: " + to_string((*tgSet1).getTeilgraphen()[0].getMaxVZhk()) << endl;


   std::cout << "MAIN was here: " + to_string(__LINE__) << endl;
   

   return 0;
}





