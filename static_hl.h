#include "global.h"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>
#include <utility>
class Static_HL
{
public:
    int n;
    vector <vector <pair<int,int> > > L;
    vector <vector <int> > pred;
    int getD(int u,int v)
    {
        map <int, int> cnt, dis;
        for(auto [x, w]: L[u])
        {
            cnt[x] += 1;
            dis[x] += w;
        }
        for(auto [x, w]: L[v])
        {
            cnt[x] += 1;
            dis[x] += w;
        }
        int ret = inf;
        for(auto [x,ct] :cnt)
            if(ct == 2)
                ret = min(ret, dis[x]);
        return ret;
    }
    vector <pair <int,int> > getSP(int u,int v)
    {
        map <int, int> cnt, dis;
        for(auto [x, w]: L[u])
        {
            cnt[x] += 1;
            dis[x] += w;
        }
        for(auto [x, w]: L[v])
        {
            cnt[x] += 1;
            dis[x] += w;
        }
        int h = 0;
        for(auto [x,ct] :cnt)
            if(ct == 2 && (h == 0 || dis[x] > dis[h]))
                h = x;
        vector <pair <int,int> > retL, retR;
        int y = u;
        while(y != h)
        {
            for(int i = 0; i < L[y].size(); i++)
                if(L[y][i].first == h)
                {
                    retL.push_back(make_pair(y, pred[y][i]));
                    y = pred[y][i];
                    break;
                }
        }
        y = v;
        while(y != h)
        {
            for(int i = 0; i < L[y].size(); i++)
                if(L[y][i].first == h)
                {
                    retR.push_back(make_pair(y, pred[y][i]));
                    y = pred[y][i];
                    break;
                }
        }
        reverse(retR.begin(), retR.end());
        for(auto it: retR) retL.push_back(it);
        return retL;
    }
    void build(Graph G)
    {
        n = G.n;
        for(int i = 0; i <= n; i++)
            L.push_back(vector<pair<int, int> >());

        vector <int> v;
        for(int i = 1; i <= n; i++)
            v.push_back(i);
        random_shuffle(v.begin(),v.end());

        for(int i = 1; i <= n; i++)
        {
            vector <vector <pair<int,int> > > tL = L;
            vector <bool> vis(n+1),d(n+1);
            for(int j = 1; j <= n; j++)
                d[j] = inf;
            d[v[i]] = 0;

            priority_queue <pair<int, int>, vector <pair<int, int> >, greater<pair<int, int> > > pq;
            pq.push(make_pair(d[v[i]], v[i]));
            vector <int> pre(n+1);
            while(!pq.empty())
            {
                int u = pq.top().second;
                if(vis[u]) continue;
                vis[u] = 1;
                if(d[u] < getD(v[i], u))
                {
                    tL[u].push_back(make_pair(v[i], d[u]));
                    pred[u].push_back(pre[u]);
                    for(auto [nxt, val]: G.ed[u])
                    {
                        if(d[u] + val < d[nxt])
                        {
                            d[nxt] = d[u] + val;
                            pre[nxt] = u;
                            pq.push(make_pair(d[nxt], nxt));
                        }
                    }
                }
            }
            L = tL;

        }
    }
};