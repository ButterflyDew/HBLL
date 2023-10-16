#pragma once
#include "global.h"
#include "betweenness_centrality.h"
class Static_HL
{
public:
    int n, siz;
    vector <vector <pair<int,int> > > L;
    vector <vector <int> > pred;
    /*获取两点间距离*/
    int getD(int u,int v);
    
    /*获取两点间最短路径（边形式）*/
    vector <pair <int,int> > getSP(int u,int v);
    
    vector <int> getuSP(int u,int v);

    void clear();
    
    void build(Graph G);

    void debug();
};
