#include "global.h"
#include <cstdio>

const int inf = 0x3f3f3f3f;

int generateRandomNumber(int l, int r)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(l, r);

    return dis(gen);
}

int Graph::get_one_num(string line)
{
    try {
        int num = std::stoi(line);
        return num;
    } catch (const invalid_argument& e) {
        cerr << "error in get_one_num" << endl;
    }
    return -1;
}

vector <int> Graph::get_num(string line)
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

void Graph::build_ed()
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

void Graph::clear()
{
    n = 0;
    gval.clear();
    edge.clear();
    ed.clear();
}

void Graph::random_graph_nm(int n_,int m_,int M)
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

void Graph::read()
{
    clear();
    string filename = "Data/graph.in";

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

void Tree::random_tree(int n_, int M)
{
    clear();
    n = n_;
    for(int i = 1; i < n; i++)
    {
        int u = generateRandomNumber(1, n);
        int v = generateRandomNumber(1, n);
        int w = generateRandomNumber(1, M);
        edge.push_back(make_pair(make_pair(u, v), w));
    }
    build_ed();
}
/*检测是否为树，待修改*/
bool Tree::check_tree()
{
    if(edge.size() != n-1) return false;
    vector <int> f(n+1);
    for(int i = 1; i <= n; i++) f[i] = i;
    auto Find = [&f](int x) {
        vector <int> rev;
        while(f[x] != x)
        {
            rev.push_back(x);
            x = f[x];
        }
        for(auto y: rev)
            f[y] = x;
        return x;
    };
    for(auto [e, w]: edge)
    {
        auto [u, v] = e;
        if(Find(u) == Find(v)) return false;
        f[Find(u)] = Find(v);
    }
    return true;
}

void Tree::clear()
{
    vex.clear();
    Id.clear();
    par.clear();
    chi.clear();
    n = rt = 0;
}

void Tree::add_vex(int u)
{
    if(Id.find(u) == Id.end()) 
    {
        Id[u] = n++;
        chi.push_back(set <int> ());
        par.push_back(-1);
        vex.insert(u);
    }    
}

void Tree::add_root(int u)
{
    rt = u;
    add_vex(u);
}

void Tree::add_edge(int u, int v)
{
    add_vex(u), add_vex(v);
    //fprintf(stderr, "addedge: %d %d\n", Id[u], Id[v]);
    chi[Id[u]].insert(Id[v]);
    par[Id[v]] = Id[u];
}

int Tree::GET_UD_T(int u,int v)
{
    map <int, int> h;
    int tu = Id[u], ct = 0;
    while(tu != Id[rt])
    {
        h[tu] = ct++;
        tu = par[tu];
    }
    h[tu] = ct++;

    int tv = Id[v]; 
    ct = 0;
    while(h.find(tv) == h.end())
    {
        ct++;
        tv = par[tv];
    }
    return h[tv] + ct;
}

void Tree::Print()
{
    map <int, int> fid;
    fprintf(stderr, "The point are: ");
    for(auto x: vex)
        fprintf(stderr, "%d ", x);
    fprintf(stderr, "\nThe edges are:\n");

    for(auto [u, nu]: Id)
        fid[nu] = u;
    for(int i = 0; i < chi.size(); i++) 
        for(auto v: chi[i])
            fprintf(stderr, "[%d %d]\n", fid[i], fid[v]);
    fprintf(stderr, "\n");
}