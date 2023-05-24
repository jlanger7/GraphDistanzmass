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
#include "knoten.h"
#include "graph.h"
#include "teilgraphenSet.h"
#include "zeitreihe.h"
#include "../tests/zeitreiheTest.h"
using namespace std;


int main(){

   vector<knoten> vec;
   knoten* i = new knoten();
   (*i).name = "jo";
   vec.push_back(*i);
   (*i).name = "no";
   delete i;
   i = NULL;
   
   knoten* k = new knoten();
   (*k).name = "heyo";

   cout << vec.back().name << endl;
   cout << (*i).name << endl;
   
   return 0;
}





