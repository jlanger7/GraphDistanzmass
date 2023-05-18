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

   vector<knoten> knotenListe;
   for(int i = 0; i < 9; i++){
      
      int zeitAttribut = 0;
      if(i < 3){
         zeitAttribut = 1;
      }else if(i < 6){
         zeitAttribut = 2;
      }else if(i < 9){
         zeitAttribut = 3;
      }
      knoten v(i, zeitAttribut);
      knotenListe.push_back(v);
   }

   cout << "Knotenliste erstellt" << endl;
   cout << size(knotenListe) << endl;

   knotenListe[0].addNachbarn(&knotenListe[3]);
   knotenListe[0].addNachbarn(&knotenListe[1]);
   knotenListe[0].addNachbarn(&knotenListe[2]);
   
   knotenListe[1].addNachbarn(&knotenListe[0]);
   knotenListe[1].addNachbarn(&knotenListe[2]);
   knotenListe[2].addNachbarn(&knotenListe[0]);
   knotenListe[2].addNachbarn(&knotenListe[1]);
   knotenListe[3].addNachbarn(&knotenListe[0]);
   knotenListe[3].addNachbarn(&knotenListe[4]);
   knotenListe[3].addNachbarn(&knotenListe[5]);
   knotenListe[4].addNachbarn(&knotenListe[3]);
   knotenListe[4].addNachbarn(&knotenListe[5]);
   knotenListe[5].addNachbarn(&knotenListe[3]);
   knotenListe[5].addNachbarn(&knotenListe[4]);
   knotenListe[5].addNachbarn(&knotenListe[6]);
   knotenListe[6].addNachbarn(&knotenListe[5]);
   knotenListe[6].addNachbarn(&knotenListe[7]);
   knotenListe[6].addNachbarn(&knotenListe[8]);
   knotenListe[7].addNachbarn(&knotenListe[6]);
   knotenListe[7].addNachbarn(&knotenListe[8]);
   knotenListe[8].addNachbarn(&knotenListe[6]);
   knotenListe[8].addNachbarn(&knotenListe[7]);

   cout << "zeiitattribut 5: " + to_string(knotenListe[5].getZeitAttribut()) << endl;
   cout << "zeiitattribut 6: " + to_string(knotenListe[6].getZeitAttribut()) << endl;
   
   graph* g = new graph(knotenListe);

   cout << "graph erstellt" << endl;

   for(int i = 0; i < size((*g).getKnotenMenge()); i++){

      knoten v = (*g).getKnotenMenge()[i];
      cout << v.getId() << endl;
   }

   teilgraphenSet* tgSet = new teilgraphenSet(g);
   cout << "teilgraphenSet erstellt" << endl;
   (*tgSet).erstelleTeilgraphenSet();

   cout << "Nachbarn 1" << endl;
   for(int i= 0; i < size(knotenListe[0].getAdjazenzListe()); i++){  
      cout << (*knotenListe[0].getAdjazenzListe()[i]).getId() << endl;
   }
   cout << "Nachbarn 2" << endl;
   for(int i= 0; i < size((*g).getKnotenMenge()[0].getAdjazenzListe()); i++){  
      cout << (*(*g).getKnotenMenge()[0].getAdjazenzListe()[i]).getId() << endl;
   }
   cout << "Nachbarn 3" << endl;
   for(int i= 0; i < size((*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[1].getAdjazenzListe()); i++){ 
      cout << "Eigene ID: " + to_string((*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].getId()) << endl;
      cout << (*(*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].getAdjazenzListe()[i]).getId() << endl;
   }
   cout << (*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].name << endl;
   (*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].loescheNtesElementAusAdjazenzListe(1);
   cout << "Nachbarn 3" << endl;
   for(int i= 0; i < 5; i++){ 
      cout << "Eigene ID: " + to_string((*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].getId()) << endl;
      cout << (*(*(*tgSet).getTeilgraphen()[0]).getKnotenMenge()[0].getAdjazenzListe()[i]).getId() << endl;
   }
   

   for(int i = 0; i < size((*tgSet).getTeilgraphen()); i++){

      cout << "Teilgraph nr: " + to_string(i+1) << endl;

      for(int z = 0; z < size((*(*tgSet).getTeilgraphen()[i]).getKnotenMenge()); z++){
         
         cout << "   KnotenId: " + to_string((*(*tgSet).getTeilgraphen()[i]).getKnotenMenge()[z].getId()) << endl;
         cout << "   KnotenZeit: " + to_string((*(*tgSet).getTeilgraphen()[i]).getKnotenMenge()[z].getZeitAttribut()) << endl;
      }
      /*(*(*tgSet).getTeilgraphen()[i]).modifizierteTiefensuche();
      cout << "Anzahl Zhks" + to_string((*(*tgSet).getTeilgraphen()[i]).getAnzahlZhk()) << endl;
      cout << "MaxVZhk" + to_string((*(*tgSet).getTeilgraphen()[i]).getMaxVZhk()) << endl;
      cout << "MinVZhk" + to_string((*(*tgSet).getTeilgraphen()[i]).getMinVZhk()) << endl;*/
   }




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





