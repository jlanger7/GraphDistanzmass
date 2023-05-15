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
using namespace std;

void test(vector<int>& z){
   
   cout << size(z) << endl;
};

int main()
{
   cout << "Hello World" << endl;

   vector<vector<int>> teilgraphenKanten;
   vector<int> f;
   f.push_back(1);
   f.push_back(2);

   cout << size(f) << endl;
   test(f);

   set<int> s;

   s.insert(30);
   s.insert(60);
   s.insert(40);
   s.insert(40);

   cout << size(s) << endl;
   set<int, greater<int> >::iterator itr;
   for (itr = s.begin(); itr != s.end(); itr++) {
        cout << *itr << " ";
    }
   

   
   return 0;
}





