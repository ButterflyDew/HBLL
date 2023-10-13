#include "static_hl.h"
#include <cstdio>
const int N=310;
int n, dis[N][N];
Graph G;
Static_HL sthl;
void floyd()
{
    n = G.n;
    for(int i = 0; i <= n+1; i++)
        for(int j = 0; j <= n+1; j++)
            dis[i][j] = inf;
    for(int i = 1; i <= n; i++)
        dis[i][i] = 0;
    for(auto [e, w]: G.edge)
    {
        auto [u, v] = e;
        dis[u][v] = min(dis[u][v], w);
        dis[v][u] = min(dis[v][u], w);
    }
    for(int k = 1; k <= n; k++)
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
}
int check()
{
    int flg = 1;
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
        {
            //fprintf(stderr, "%d %d\n", i, j);
            if(dis[i][j] != sthl.getD(i, j))
            {
                fprintf(stderr, "[%d %d] = %d, not %d\n",i, j, dis[i][j], sthl.getD(i, j));
                flg = 0;
            }  
            if(dis[i][j] == inf) continue;
            vector <pair <int, int> > route = sthl.getSP(i, j);
            int sum = 0;
            for(auto it: route)
            {
                if(G.gval.find(it) == G.gval.end())
                {
                    fprintf(stderr, "the edge(%d, %d) is not exist!\n", it.first, it.second);
                    flg = 0;
                }
                else sum += G.gval[it];
            }
            if(sum != dis[i][j])
            {
                fprintf(stderr, "the route length (%d) is not equal to the dis (%d) of %d and %d!\n",sum, dis[i][j], i, j);
                flg = 0;
            }
        }    
    return flg;  
}
void check_with_floyd()
{
    int n = 100, m = 1000, M = 200;
    for(int i = 1; i <= 100; i++)
    {
        G.random_graph_nm(n, m, M);
        //G.Print();
        floyd();
        sthl.build(G);
        //sthl.debug();
        if(check() == 0)
        {
            G.Print();
            sthl.debug();
            printf("err in %d\n", i);
            break;
        }    
        printf("ok in %d\n", i);
    }
}
void check_label_size()
{
    auto start_time = chrono::high_resolution_clock::now();
        
    int n = 10000, m = 3*n, M = 200;
    G.random_graph_nm(n, m, M);
    sthl.build(G);
    cerr << "siz:" << sthl.siz << endl;

    auto end_time = chrono::high_resolution_clock::now();
    auto exec = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "init time:" << exec.count()/1e6 << "s" << endl;


    start_time = chrono::high_resolution_clock::now();

    int T = 100000;
    for(int i = 1; i <= T; i++)
    {
        int u = generateRandomNumber(1, n);
        int v = generateRandomNumber(1, n);
        sthl.getD(u, v);
    }
    
    end_time = chrono::high_resolution_clock::now();
    exec = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "query time:" << exec.count()/1e6 << "s" << endl;
}
int main()
{
    /*G.read();
    floyd();
    sthl.build(G);
    sthl.debug();
    check();*/
    //check_with_floyd();
    check_label_size();
    return 0;
}