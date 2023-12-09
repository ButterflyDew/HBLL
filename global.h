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
#include <sys/stat.h> 
#include <filesystem>
using namespace std;

extern const int inf;
extern const double INF;
extern const double eps;
int generateRandomNumber(int l, int r); 
double getRandomRealNumber(double l, double r);
vector<int> extractIntegers(const string& input);
double get_now_time();

struct Triple
{
    int l, h;
    double d;
    Triple(){l = h = d = 0;}
    Triple(int L, int H, double D): l(L), h(H), d(D) {}
};

class Graph
{
private:
    double get_one_double(string line);
    int get_one_num(string line);

    pair < pair <int, int> , double > get_edge(string line);

    vector <int> get_num(string line);
public:
    int n;
    // 先读 edge，然后以 edge 给下面两个
    vector <pair <pair<int, int>, double> > edge;
    vector <vector < pair <int, double> > > ed;
    map <pair <int, int>, double > gval;

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

    void random_graph_real(int n_, int m_, double M);

    void read(string filename, int typ);
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

    void Print_to_file(string prefile, int qid, int D, vector <int> M);

    void random_tree(int n_, int M);
    
    bool check_tree();
    
    void add_root(int u); 

    void add_vex(int u);
    //有向边
    void add_edge(int u, int v);

    int GET_UD_T(int u,int v);

    double get_sumvalue();
};