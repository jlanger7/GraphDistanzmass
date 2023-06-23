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

void test(int* z){
   
   int* i = z;
   *i +=1 ; 
   cout << *z << endl;
   //z += 1;
};

int main()
{

   
   auto now = std::chrono::system_clock::now();
   auto in_time_t = std::chrono::system_clock::to_time_t(now);
   std::stringstream datetime;
   datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M");
    
   double a = 5.68492757697;
   float f = 5.68492757697;
   cout << a << endl;
   cout << f << endl; 
   
   
   return 0;
}





