#ifndef KNOTEN_H
#define KNOTEN_H
#include <vector>
#include <set>
using namespace std;

class knoten{

    private:
    int id;
    int zeitAttribut;
    int nrZHK = -1;
    vector<int> adjazenzListe; //enthält IDs der Nachbarknoten


    public:
    knoten();
    knoten(int idIn, int zeitAttributIn);
    void addNachbarn(int nachbarId);
    
    void setId(int idIn){
        id = idIn;
    };
    int getId(){
        return id;
    };
    void setZeitAttribut(int zeitAttributIn){
        zeitAttribut = zeitAttributIn;
    };
    int getZeitAttribut(){
        return zeitAttribut;
    };
    void setAdjazenzListe(vector<int> adjazenzListe){
        adjazenzListe = adjazenzListeIn;
    };



};

#endif