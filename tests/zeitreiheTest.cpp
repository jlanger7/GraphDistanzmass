#include "../src/zeitreihe.h"
#include "zeitreiheTest.h"
#include <vector>
#include <iostream>
using namespace std;

zeitreiheTest::zeitreiheTest(){

};

void zeitreiheTest::setInputGraphen(){

    knoten.push_back(1);
    knoten.push_back(1);
    knoten.push_back(1);
    knoten.push_back(2);
    knoten.push_back(2);
    knoten.push_back(2);
    knoten.push_back(3);
    knoten.push_back(3);
    knoten.push_back(3);

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
    kanten.push_back(5);
    kanten.push_back(7);
    kanten.push_back(6);
    kanten.push_back(7);
    kanten.push_back(6);
    kanten.push_back(8);
    kanten.push_back(7);
    kanten.push_back(8);

};

bool zeitreiheTest::testeErstelleTeilgraphenSet(){

    setInputGraphen();
    zeitreihe* z = new zeitreihe(knoten, kanten);

    (*z).printTeilgraphKnotenUndKanten();

    set<int, greater<int> >::iterator itr;
    int i = 0;
    for(itr = (*z).getTeilgraphenKnoten()[0].begin(); itr != (*z).getTeilgraphenKnoten()[0].end(); itr++) {
        
        if(i == 0 && *itr == 0 || i == 1 && *itr == 1 || i == 2 && *itr == 2){
            i += 1;
        }else {
             return false;
        }
    }
    i = 0;
    for(itr = (*z).getTeilgraphenKnoten()[1].begin(); itr != (*z).getTeilgraphenKnoten()[1].end(); itr++) {
        
        if(i == 0 && *itr == 3 || i == 1 && *itr == 4 || i == 2 && *itr == 5){
            i += 1;
        }else {
             return false;
        }
    }
    i = 0;
    for(itr = (*z).getTeilgraphenKnoten()[2].begin(); itr != (*z).getTeilgraphenKnoten()[2].end(); itr++) {
        
        if(i == 0 && *itr == 6 || i == 1 && *itr == 7 || i == 2 && *itr == 8){
            i += 1;
        }else {
             return false;
        }
    }
    
    //Kanten testen

    /*for(int i = 0; i < size((*z).getTeilgraphenKnoten()); i++){

        
        
        cout << "Teilgraph Nr.: " + to_string(i+1) << endl;
        cout << "   Knoten: " << endl;
        set<int, greater<int> >::iterator itr;
        for(itr = (*z).getTeilgraphenKnoten()[i].begin(); itr != (*z).getTeilgraphenKnoten()[i].end(); itr++) {
                cout << "   " << *itr << " ";
        }
        cout << "   Kanten: " << endl;
        for(int e = 0; e < size((*z).getTeilgraphenKanten()[i]); e+=2){

            cout << "   (" + to_string((*z).getTeilgraphenKanten()[i][e]) + "," + to_string((*z).getTeilgraphenKanten()[i][e+1]) + ")" << endl;
        }
    }*/
    return true;
};