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

#include "zeitreihe.h"
#include "../tests/zeitreiheTest.h"
using namespace std;

void test(vector<int>& z){
   
   cout << size(z) << endl;
};

int main()
{
   cout << "Hello World" << endl;

   vector<int>* knoten = new vector<int>;
   cout << "was ist hier" << endl;
   (*knoten).push_back(1);
   (*knoten).push_back(2);
   cout << "was ist hier1" << endl;
   cout << (*knoten)[0] << endl;
   cout << (*knoten)[1] << endl;

   cout << "was ist hier2" << endl;

   delete knoten;

   int a = 10;
   int b = 30029;
   int c= 45838395;

   cout << (*knoten)[0] << endl;
   cout << (*knoten)[1] << endl;

  /* vector<vector<int>> teilgraphenKanten;
   vector<int> knoten;
   knoten.push_back(1);
   knoten.push_back(1);
   knoten.push_back(1);
   knoten.push_back(2);
   knoten.push_back(2);
   knoten.push_back(2);
   knoten.push_back(3);
   knoten.push_back(3);
   knoten.push_back(3);

   vector<int> kanten;  
   kanten.push_back(0);
   kanten.push_back(1);
   kanten.push_back(0);
   kanten.push_back(2);
   kanten.push_back(1);
   kanten.push_back(2);
   kanten.push_back(2);
   kanten.push_back(3);
   kanten.push_back(3);
   kanten.push_back(4);
   kanten.push_back(3);
   kanten.push_back(5);
   kanten.push_back(4);
   kanten.push_back(5);
   kanten.push_back(6);
   kanten.push_back(7);
   kanten.push_back(6);
   kanten.push_back(8);
   kanten.push_back(7);
   kanten.push_back(8);

   zeitreihe* z = new zeitreihe(knoten, kanten);

   z->printTeilgraphKnotenUndKanten();
   */
   
   /*Set Umgang
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
   */

   
   return 0;
}





