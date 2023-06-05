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
   graph* g1 = new graph(knotenListe1);
   (*g1).berechneKanten(txt);

   return 0;
}





