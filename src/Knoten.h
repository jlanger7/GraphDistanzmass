#ifndef knoten_H
#define knoten_H
#include <vector>
#include <set>
using namespace std;

class knoten{

    private:
    int id;
    int zeitAttribut;
    int nrZHK = -1;
    vector<knoten*> adjazenzListe; //enthält IDs der Nachbarknoten


    public:
    knoten();
    knoten(int idIn, int zeitAttributIn);
    void addNachbarn(knoten* nachbar);
    
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
    void setAdjazenzListe(vector<knoten*> adjazenzListeIn){
        adjazenzListe = adjazenzListeIn;
    };
    vector<knoten*>& getAdjazenzListe(){
        return adjazenzListe;
    };



};
#endif