#include "../src/zeitreihe.h"
#include "zeitreiheTest.h"
#include <vector>
#include <iostream>
using namespace std;

int main(){

    zeitreihe p;
    vector<vector<int>> pvec;
    vector<int> ivec;
    ivec.push_back(1);
    ivec.push_back(1);
    ivec.push_back(1);
    pvec.push_back(ivec);
    pvec.push_back(ivec);
    ivec[0] = 2;
    ivec[1] = 2;
    ivec[2] = 2;
    pvec.push_back(ivec);
    ivec[0] = 3;
    ivec[1] = 3;
    ivec[2] = 3;
    pvec.push_back(ivec);
    ivec[0] = 5;
    ivec[1] = 5;
    ivec[2] = 5;
    pvec.push_back(ivec);
    ivec[0] = 4;
    ivec[1] = 4;
    ivec[2] = 4;
    pvec.push_back(ivec);
    p.setZeitreihenWerte(pvec);


    zeitreihe q;
    vector<vector<int>> qvec;
    ivec[0] = 1;
    ivec[1] = 1;
    ivec[2] = 1;
    qvec.push_back(ivec);
    ivec[0] = 5;
    ivec[1] = 5;
    ivec[2] = 5;
    qvec.push_back(ivec);
    ivec[0] = 3;
    ivec[1] = 3;
    ivec[2] = 3;
    qvec.push_back(ivec);
    q.setZeitreihenWerte(qvec);

    cout << p.berechneDiskreteFrechetDistanz(q.getZeitreihenWerte()) << endl;
    cout << p.berechneDtwDistanz(q.getZeitreihenWerte()) << endl;

    return 0;
}