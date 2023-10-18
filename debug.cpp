#include "static_hl.h"
#include "hbll.h"
#include "betweenness_centrality.h"
#include <cstdio>
#include <utility>
const int N=310;
int n, dis[N][N];
Graph G;
Static_HL sthl;
HBLL hbll;
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
void check_B_C()
{
    //int n = 4, m = 5, M = 1;
    //G.random_graph_nm(n, m, M);
    G.read();
    B_C betc;
    betc.build(G);
}
int dp[101][101][101];
void go_hbll_bf()
{
    n = G.n;
    for(int h = 0; h <= n; h++)
        for(int i = 0; i <= n+1; i++)
            for(int j = 0; j <= n+1; j++)
                dp[h][i][j] = inf;
    for(int r = 1; r <= n; r++) dp[0][r][r] = 0;

    for(int h = 0; h < n; h++)
        for(int r = 1; r <= n; r++)
            for(int u = 1; u <= n; u++)
                for(auto [v, w]: G.ed[u])
                    if(dp[h+1][r][v] > dp[h][r][u] + w)
                        dp[h+1][r][v] = dp[h][r][u] + w;

    for(int h = 1; h <= n; h++)
    {
        for(int i = 0; i <= n+1; i++)
            for(int j = 0; j <= n+1; j++)
                dp[h][i][j] = min(dp[h][i][j], dp[h-1][i][j]);
    }
}
void check_hbll()
{

    int n = 60, m = 120, M = 10;
    for(int T = 1; T <= 100; T++)
    {
        G.random_graph_nm(n, m, M);
        // G.read();
        // int n = G.n;

        go_hbll_bf();
        hbll.build_hbll(G);
    
        //fprintf(stderr, "hbll has been built!\n");
        //hbll.output_L();
        for(int h = 1; h <= n-1; h++)
            for(int i = 1; i <= n; i++)
                for(int j = 1; j <= n; j++)
                {
                    //fprintf(stderr, "Testing [%d, (%d, %d)]\n", h, i, j);
                    if(dp[h][i][j] != hbll.GET_WD(i, j, h))
                    {
                        fprintf(stderr, "[%d %d %d] = %d, not = %d!\n", h, i, j, dp[h][i][j], hbll.GET_WD(i, j, h));
                        G.Print();
                        return;
                    }
                    if(dp[h][i][j] == inf) continue;
                    //cout << "222" << endl;
                    auto route = hbll.GET_MWP(i, j, h);
                    //cout << "out of getmwp" << endl;
                    if(route.front() != i || route.back() != j)
                    {   
                        fprintf(stderr, "the route's begin or end is err!\n");
                        G.Print();
                        return;
                    }
                    //cout << "2222" << endl;
                    if(route.size() > h + 1)
                    {
                        fprintf(stderr, "the route's length exceeds %d!\n", h);
                        G.Print();
                        return;
                    }
                    //cout << "22222" << endl;
                    int las = 0, sum = 0;
                    for(auto u: route)
                    {
                        if(las != 0)
                        {
                            auto it = make_pair(las, u);
                            if(G.gval.find(it) == G.gval.end())
                            {
                                fprintf(stderr, "the edge(%d, %d) is not exist!\n", it.first, it.second);
                                G.Print();
                                return;
                            }
                            else
                                sum += G.gval[it];
                        }
                        las = u;
                    }
                    //cout << "222222" << endl;
                    if(sum != dp[h][i][j])
                    {
                        fprintf(stderr, "the route length (%d) is not equal to the dis (%d) of [%d, %d, %d]!\n",sum, dis[i][j], h, i, j);
                        G.Print();
                        return;
                    }
                }

        fprintf(stderr, "ok in %d!\n", T);
    }
   
}
int main()
{
    /*G.read();
    floyd();
    sthl.build(G);
    sthl.debug();
    check();*/
    //check_with_floyd();
    //check_label_size();
    //check_B_C();
    check_hbll();
    return 0;
}