#pragma once
#include <iostream>
#include <cstdio>
#include <random>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <chrono>
#include <set>
#include <sstream>
using namespace std;

extern const int inf;

int generateRandomNumber(int l, int r); 

class Graph
{
private:
    int get_one_num(string line);

    vector <int> get_num(string line);
public:
    int n;
    // 先读 edge，然后以 edge 给下面两个
    vector <pair <pair<int, int>, int> > edge;
    vector <vector < pair <int, int> > > ed;
    map <pair <int, int>, int > gval;

    Graph(){clear();}

    void Print()
    {
        cerr << "n: " << n << endl;
        for(int i = 1; i <= n; i++)
            for(auto [v, w]: ed[i])
                cerr << "[u, v, w] = " << i << "," << v << "," << w << endl;
    }

    void build_ed();

    void clear();

    void random_graph_nm(int n_, int m_, int M);

    void read();
};


class Tree: public Graph
{
public:
    set <int> vex;
    //对点离散化
    map <int,int> Id;
    //par and chi 全身存储离散化后的
    vector <int> par;
    vector <set <int> > chi;
    // n 从 0 开始，rt 是离散化前的根
    int n, rt;
    Tree(){clear();}

    void clear();
    
    void Print();

    void random_tree(int n_, int M);
    
    bool check_tree();
    
    void add_root(int u);

    void add_vex(int u);
    //有向边
    void add_edge(int u, int v);

    int GET_UD_T(int u,int v);
};