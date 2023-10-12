#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>
using namespace std;
extern const int inf = 0x3f3f3f3f;
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
            }
        } catch (const invalid_argument& e) {
            cerr << "error in get_two_num" << endl;
        }
        return ret;
    }
public:
    int n;
    vector <pair <pair<int, int>,int> > edge;
    vector <vector < pair <int,int> > > ed;
    void builded()
    {
        for(int i = 0; i <= n; i++)
            ed.push_back(vector < pair <int,int> > ());
        for(auto [e, w]: edge)
        {
            auto [u, v] = e;
            ed[u].push_back(make_pair(v, w));
            ed[v].push_back(make_pair(u, w));
        }
    }
    void read()
    {
        string filename = "Data/graph.in";

        ifstream inputFile(filename);

        if (!inputFile.is_open()) 
            cerr << "无法打开文件: " << filename << endl;

        string line;
        getline(inputFile, line);

        n = get_one_num(line);

        while (getline(inputFile, line)) 
        {
            //cout << line << endl;
            auto num = get_num(line);
            edge.push_back(make_pair(make_pair(num[0], num[1]), num.size()==2? 1: num[3]));
        }
        build_ed();
        inputFile.close();
    }
}G;