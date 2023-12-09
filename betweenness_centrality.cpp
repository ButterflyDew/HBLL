#include "betweenness_centrality.h"

void B_C::sol(int rt, Graph &G)
{
    priority_queue <pair<double, int>, vector <pair<double, int> >, greater<pair<double, int> > > pq;
    vector <double> d(n+1);
    vector <int> vis(n+1),topo;
    vector <vector <int> > prev,sucv;
    vector <double> sv(n+1);
    for(int i = 0; i <= n; i++) 
    {
        d[i] = INF;
        prev.push_back(vector <int> ());
        sucv.push_back(vector <int> ());
    }
    pq.push(make_pair(d[rt] = 0, rt));
    sv[rt] = 1;
    while(!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        topo.push_back(u);
        for(auto [v, w]: G.ed[u])
        {
            if(d[v] > d[u] + w)
            {
                d[v] = d[u] + w;
                prev[v].clear();
                prev[v].push_back(u);
                sv[v] = 0;
                sv[v] += sv[u];
                pq.push(make_pair(d[v], v));
            }
            else if(d[v] == d[u] + w)
            {
                prev[v].push_back(u);
                sv[v] += sv[u];
            }    
        }
    }
    for(int v = 1; v <= n; v++)
        for(auto u: prev[v])
            sucv[u].push_back(v);
    
    vector <double> dp(n+1);
    for(int i = (int)(topo.size()) - 1; i >= 0; i--)
    {
        int u = topo[i];
        for(auto v: sucv[u])
            dp[u] += sv[u]/sv[v]*(1 + dp[v]);
    }

    // fprintf(stderr, "s is %d:\n",rt);
    // for(int i = 1; i <= n; i++) 
    //     fprintf(stderr, "i: %d cnt: %.0lf dp: %.5lf\n", i, sv[i], dp[i]);
    // fprintf(stderr, "\n");

    for(int i = 1; i <= n; i++) 
        if(i != rt)
            BC[i] += dp[i];
}


void B_C::build(Graph G)
{
    n = G.n;
    for(int i = 0; i <= n; i++) BC.push_back(0);
    for(int i = 1; i <= n; i++) sol(i, G);
    BC[0] = inf;
    // fprintf(stderr, "BC is :");
    // for(int i = 1; i <= n; i++)
    //     fprintf(stderr, "%.10lf ",BC[i]);
    // fprintf(stderr, "\n");
}