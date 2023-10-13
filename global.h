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
using namespace std;
extern const int inf = 0x3f3f3f3f;

int generateRandomNumber(int l, int r) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(l, r);

    return dis(gen);
}

class Graph
{
private:
    int get_one_num(string line)
    {
        try {
            int num = std::stoi(line);
            return num;
        } catch (const invalid_argument& e) {
            cerr << "error in get_one_num" << endl;
        }
        return -1;
    }
    vector <int> get_num(string line)
    {
        vector <int> ret;
        try{
            int pos;
            while((pos = line.find(" "))!=string::npos)
            {
                int u = get_one_num(line.substr(0,pos));
                ret.push_back(u);
                line = line.substr(pos+1);
                //cout << line << endl;
            }
            ret.push_back(get_one_num(line));
        } catch (const invalid_argument& e) {
            cerr << "error in get_two_num" << endl;
        }
        return ret;
    }
public:
    int n;
    // 先读 edge，然后以 edge 给下面两个
    vector <pair <pair<int, int>, int> > edge;
    vector <vector < pair <int, int> > > ed;
    map <pair <int, int>, int > gval;
    void Print()
    {
        cerr << "n: " << n << endl;
        for(int i = 1; i <= n; i++)
            for(auto [v, w]: ed[i])
                cerr << "[u, v, w] = " << i << "," << v << "," << w << endl;
    }
    void build_ed()
    {
        for(int i = 0; i <= n; i++)
            ed.push_back(vector < pair <int,int> > ());
        for(auto [e, w]: edge)
        {
            auto [u, v] = e;
            //cout << "[" << u << "," << v << "," << w << "]" << endl;
            auto ve = make_pair(v, u);
            if(gval.find(e) != gval.end())
            {
                gval[e] = min(gval[e], w);
                gval[ve] = min(gval[ve], w);
            }
            else
            {
                gval[e] = w;
                gval[ve] = w;
            }

            ed[u].push_back(make_pair(v, w));
            ed[v].push_back(make_pair(u, w));
        }
    }
    void clear()
    {
        n = 0;
        gval.clear();
        edge.clear();
        ed.clear();
    }
    void random_graph_nm(int n_,int m_,int M)
    {
        clear();
        n = n_;
        for(int i = 1; i <= m_; i++)
        {
            int u = generateRandomNumber(1, n);
            int v = generateRandomNumber(1, n);
            int w = generateRandomNumber(1, M);
            edge.push_back(make_pair(make_pair(u, v), w));
        }
        build_ed();
    }
    void read()
    {
        clear();
        string filename = "Data/debug.in";

        ifstream inputFile(filename);

        if (!inputFile.is_open()) 
            cerr << "Can't open file: " << filename << endl;

        string line;
        getline(inputFile, line);

        n = get_one_num(line);

        while (getline(inputFile, line)) 
        {
            //cout << line << endl;
            auto num = get_num(line);
            if(num.size() == 2 || num.size() == 3)
                edge.push_back(make_pair(make_pair(num[0], num[1]), num.size()==2? 1: num[2]));
            else
            {
                cerr << "Input invalid!" << endl;
                exit(0);
            }
        }
        build_ed();
        inputFile.close();
    }
};
