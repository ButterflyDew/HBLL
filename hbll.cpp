#include "hbll.h"
#include <cstdio>
#include <utility>

void HBLL::clear()
{
    n = 0;
    L.clear();
    prew.clear();
}
void HBLL::build_hbll(Graph G)
{
    clear();
    n = G.n;

    vector <int> v;
    for(int i = 0; i <= n; i++)
    {
        v.push_back(i);
        L.push_back(vector <Triple> ());
        prew.push_back(vector <pair <int,int > > ());
    }    
    B_C betc;
    betc.build(G);
    sort(v.begin(), v.end(), [&betc](int v1, int v2) {
        return betc.BC[v1] > betc.BC[v2];
    });

    for(int i = 1; i <= n; i++)
    {
        //以 v[i] 为根， d[h][i] 带权距离 V[h] 在此层的点
        vector <int> id(n+1), d[2], V[2];
        for(int j = 1; j <= n; j++) 
            id[j] = L[j].size();

        V[0].push_back(v[i]);
        for(int l = 0; l <= 1; l++)
            for(int j = 0; j <= n; j++)
                d[l].push_back(inf);
         
        d[0][v[i]] = 0;
        vector <pair <int, int > > prelas(n+1);
        prelas[v[i]] = make_pair(0, 0);
        for(int h = 0; ; h++)
        {
            int cur = h&1;
            if(V[cur].size() == 0) break;

            sort(V[cur].begin(), V[cur].end());
            V[cur].erase(unique(V[cur].begin(), V[cur].end()), V[cur].end());

            V[cur^1].clear();
            d[cur^1] = d[cur];
            vector <pair <int, int> > pre(n+1);// [par, h]
            for(auto u: V[cur])
            {
                if(d[cur][u] >= GET_WD_build(v[i], u, h, id).first)
                    continue;
                L[u].push_back(Triple(v[i], h, d[cur][u]));
                prew[u].push_back(prelas[u]);
                for(auto [nxt, w]: G.ed[u])
                {
                    if(d[cur][u] + w < d[cur^1][nxt])
                    {
                        V[cur^1].push_back(nxt);
                        d[cur^1][nxt] = d[cur][u] + w;
                        pre[nxt] = make_pair(u, h);
                    }
                }
            }
            prelas = pre;
        }
    }
}
// [dis, midx, h1, h2]
pair <int,pair <int, pair <int,int> > > HBLL::GET_WD_build(int u, int v, int h, vector <int> id = vector <int>())
{
    int nu = L[u].size(), nv = L[v].size(); 
    if(!id.empty()) 
        nu = id[u], nv = id[v];

    // pot[typ][vid[typ][vv]] -> vector <pair <int,int> > which root is vv
    map <int,int> vid[2];
    vector <vector <pair <int,int> > > pot[2];
    for(int i = 0; i <= 1; i++)
        pot[i].push_back(vector <pair <int,int> >());

    for(int i = 0; i < nu; i++)
    {
        auto it = L[u][i];
        if(it.h > h) continue;
        if(vid[0][it.l] == 0)
        {
            pot[0].push_back(vector <pair <int,int> >());
            vid[0][it.l] = pot[0].size() - 1;
        }    
            
        pot[0][vid[0][it.l]].push_back(make_pair(it.h, it.d));
    }

    for(int i = 0; i < nv; i++)
    {
        auto it = L[v][i];
        if(it.h > h) continue;
        if(vid[1][it.l] == 0)
        {
            pot[1].push_back(vector <pair <int,int> >());
            vid[1][it.l] = pot[1].size() - 1;
        }    
            
        pot[1][vid[1][it.l]].push_back(make_pair(it.h, it.d));
    }
    int ans = inf, idl = 0, h1 = 0, h2 = 0;
    for(auto [l, pu]: vid[0])
    {
        if(vid[1].find(l) == vid[1].end()) continue;
        int pv = vid[1][l]; 
        auto hdu = pot[0][pu];
        auto hdv = pot[1][pv];
        if(hdu.empty() || hdv.empty()) continue;
        sort(hdu.begin(), hdu.end());
        sort(hdv.begin(), hdv.end());
        vector <int> miv, mivid;
        for(auto [hv, d]: hdv)
        {
            if(miv.empty()) miv.push_back(d), mivid.push_back(hv);
            else 
            {
                mivid.push_back(miv.back() <= d ? mivid.back(): hv);
                miv.push_back(min(miv.back(), d));
            }
        }
        int j = hdv.size() - 1;
        for(auto [hu, d]: hdu)
        {
            while(j >= 0 && hu + hdv[j].first > h) --j;
            if(j == -1) break;
            if(ans > d + miv[j])
            {
                ans = d + miv[j];
                idl = l;
                h1 = hu;
                h2 = mivid[j];
            }
        }
    }
    return make_pair(ans, make_pair(idl, make_pair(h1, h2)));
}

int HBLL::GET_UD(int u, int v)
{
    map <int, int > hm;
    for(auto it: L[u])
        hm[it.l] = it.h;
    int ans = inf;
    for(auto it: L[v])
        if(hm.find(it.l) != hm.end())
            ans = min(ans, hm[it.l] + it.h);
    return ans;
}

int HBLL::GET_WD(int u, int v, int h)
{
    return GET_WD_build(u, v, h).first;
}

vector <int> HBLL::GET_MWP(int u, int v, int h)
{
    auto info = GET_WD_build(u, v, h).second;
    int idl = info.first;
    auto [h1, h2] = info.second;
    int y = u;
    vector <int> retL, retR;
    fprintf(stderr, "u, v, h = [%d, %d, %d], idl, h1, h2 = [%d, %d, %d]\n", u, v, h, idl, h1, h2);
    while(y != idl)
    {
        fprintf(stderr, "%d ", y);
        for(int i = 0; i < L[y].size(); i++)
            if(L[y][i].l == idl && L[y][i].h == h1)
            {
                retL.push_back(y);
                y = prew[y][i].first;
                h1 = prew[y][i].second;
                break;
            }
    }
    y = v;

    while(y != idl)
    {
        //fprintf(stderr, "in rig");
        for(int i = 0; i < L[y].size(); i++)
            if(L[y][i].l == idl && L[y][i].h == h2)
            {
                retR.push_back(y);
                y = prew[y][i].first;
                h2 = prew[y][i].second;
                break;
            }
    }
    retL.push_back(idl);
    reverse(retR.begin(), retR.end());
    for(auto it: retR) retL.push_back(it);
    return retL;
}


void HBLL::output_L()
{
    int sum = 0;
    for(auto Lu: L) sum += Lu.size();
    printf("sum of L is %d\n", sum);
    for(int i = 1; i <= n; i++)
    {
        printf("in %d:\n", i);
        for(auto it: L[i])
            printf("[%d, %d, %d]\n", it.l, it.h, it.d);
        puts("");
    }
}