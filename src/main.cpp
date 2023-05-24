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




/*
   int a = 5;
   test(&a);
   cout << a << endl;
*/
   cout << "Hello World" << endl;

   vector<knoten*> knotenListe;
   for(int i = 0; i < 9; i++){
      
      int zeitAttribut = 0;
      if(i < 3){
         zeitAttribut = 1;
      }else if(i < 6){
         zeitAttribut = 2;
      }else if(i < 9){
         zeitAttribut = 3;
      }
      knoten* v = new knoten(i, zeitAttribut);
      knotenListe.push_back(v);
   }
   knoten* v = new knoten(9, 1);
   knotenListe.push_back(v);

   graph* g = new graph(knotenListe);

   (*(*g).getKnotenMenge()[0]).addNachbarn((*g).getKnotenMenge()[3]);//extra
   (*(*g).getKnotenMenge()[0]).addNachbarn((*g).getKnotenMenge()[4]);//extra
   (*(*g).getKnotenMenge()[0]).addNachbarn((*g).getKnotenMenge()[1]);
   (*(*g).getKnotenMenge()[0]).addNachbarn((*g).getKnotenMenge()[2]);
   (*(*g).getKnotenMenge()[1]).addNachbarn((*g).getKnotenMenge()[0]);
   (*(*g).getKnotenMenge()[1]).addNachbarn((*g).getKnotenMenge()[2]);
   (*(*g).getKnotenMenge()[2]).addNachbarn((*g).getKnotenMenge()[0]);
   (*(*g).getKnotenMenge()[2]).addNachbarn((*g).getKnotenMenge()[8]);//extra
   (*(*g).getKnotenMenge()[2]).addNachbarn((*g).getKnotenMenge()[1]);
   (*(*g).getKnotenMenge()[3]).addNachbarn((*g).getKnotenMenge()[0]);//extra
   (*(*g).getKnotenMenge()[3]).addNachbarn((*g).getKnotenMenge()[4]);
   (*(*g).getKnotenMenge()[3]).addNachbarn((*g).getKnotenMenge()[5]);
   (*(*g).getKnotenMenge()[4]).addNachbarn((*g).getKnotenMenge()[3]);
   (*(*g).getKnotenMenge()[4]).addNachbarn((*g).getKnotenMenge()[5]);
   (*(*g).getKnotenMenge()[5]).addNachbarn((*g).getKnotenMenge()[3]);
   (*(*g).getKnotenMenge()[5]).addNachbarn((*g).getKnotenMenge()[4]);
   (*(*g).getKnotenMenge()[5]).addNachbarn((*g).getKnotenMenge()[6]);//extra
   (*(*g).getKnotenMenge()[6]).addNachbarn((*g).getKnotenMenge()[5]);//extra
   (*(*g).getKnotenMenge()[6]).addNachbarn((*g).getKnotenMenge()[7]);
   (*(*g).getKnotenMenge()[6]).addNachbarn((*g).getKnotenMenge()[8]);
   (*(*g).getKnotenMenge()[7]).addNachbarn((*g).getKnotenMenge()[6]);
   (*(*g).getKnotenMenge()[7]).addNachbarn((*g).getKnotenMenge()[8]);
   (*(*g).getKnotenMenge()[8]).addNachbarn((*g).getKnotenMenge()[6]);
   (*(*g).getKnotenMenge()[8]).addNachbarn((*g).getKnotenMenge()[7]);
   (*(*g).getKnotenMenge()[8]).addNachbarn((*g).getKnotenMenge()[2]);//extra

   for(int i = 0; i < size((*g).getKnotenMenge()); i++){

      knoten v = (*(*g).getKnotenMenge()[i]);
      cout << v.getId() << endl;
   }

   teilgraphenSet* tgSet = new teilgraphenSet(g);
   (*tgSet).erstelleTeilgraphenSet();

   /*
   cout << "Nachbarn 3" << endl;
   cout << "Eigene ID: " + to_string((*(*tgSet).getTeilgraphen()[0].getKnotenMenge()[0]).getId()) << endl;
   for(int i= 0; i < size((*(*tgSet).getTeilgraphen()[0].getKnotenMenge()[1]).getAdjazenzListe()); i++){ 
      cout << (*(*(*tgSet).getTeilgraphen()[0].getKnotenMenge()[0]).getAdjazenzListe()[i]).getId() << endl;
   }
   */
   

   for(int i = 0; i < size((*tgSet).getTeilgraphen()); i++){

      //cout << "Teilgraph nr: " + to_string(i+1) << endl;
      /*
      for(int z = 0; z < size((*tgSet).getTeilgraphen()[i].getKnotenMenge()); z++){
         
         cout << "   KnotenId: " + to_string((*(*tgSet).getTeilgraphen()[i].getKnotenMenge()[z]).getId()) << endl;
         cout << "   KnotenZeit: " + to_string((*(*tgSet).getTeilgraphen()[i].getKnotenMenge()[z]).getZeitAttribut()) << endl;
         cout << "   Nachbarn: " << endl;
         for(int y= 0; y < size((*(*tgSet).getTeilgraphen()[i].getKnotenMenge()[z]).getAdjazenzListe()); y++){ 
            cout << "      " + to_string((*(*(*tgSet).getTeilgraphen()[i].getKnotenMenge()[z]).getAdjazenzListe()[y]).getId()) << endl;
         }
      }
      */
      (*tgSet).getTeilgraphen()[i].modifizierteTiefensuche();
      //cout << "Anzahl Zhks: " + to_string((*tgSet).getTeilgraphen()[i].getAnzahlZhk()) << endl;
      //cout << "MaxVZhk: " + to_string((*tgSet).getTeilgraphen()[i].getMaxVZhk()) << endl;
      //cout << "MinVZhk: " + to_string((*tgSet).getTeilgraphen()[i].getMinVZhk()) << endl;
   }




  

   zeitreihe z(tgSet);
   z.printZeitreihe();

   
   return 0;
}





