#pragma once
#include "global.h"
#include "betweenness_centrality.h"
class HBLL
{
public:
    int cntwd, cntud;
    long long pcntwd, pcntud;
    void clearcnt();
    void printcnt();
    void clearbuc();
    void rearrange();
    int n;
    vector < vector <Triple> > L;
    vector < vector <int> > prew;
    void build_hbll(Graph G);
    pair <double ,pair <int, pair <int,int> > > GET_WD_build(int u, int v, int h, vector <int> id);
    double GET_WD(int u, int v, int h);
    int GET_UD(int u, int v);
    vector <int> GET_MWP(int u, int v, int h);
    void output_L();
    void output_F(string filepre);
    double Average_L();
    void clear();
};

