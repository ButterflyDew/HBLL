#include "static_hl.h"

int Static_HL::getD(int u,int v)
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
    for(auto [x, ct] :cnt)
        if(ct == 2)
            ret = min(ret, dis[x]);
    return ret;
}

vector <pair <int,int> > Static_HL::getSP(int u,int v)
{
    vector <pair <int,int> > retL, retR;
    if(u == v)
        return retL;
    map <int, int> cnt, dis;
    //cerr << "L[u]:" << endl;
    for(auto [x, w]: L[u])
    {
        cnt[x] += 1;
        dis[x] += w;
        //cerr << "[x, w] = " << x << " " << w << " dis[x] " << dis[x] <<endl;
    }
    //cerr << "L[v]:" << endl;
    for(auto [x, w]: L[v])
    {
        cnt[x] += 1;
        dis[x] += w;
        //cerr << "[x, w] = " << x << " " << w << " dis[x] " << dis[x] <<endl;
    }
    int h = 0;
    for(auto [x, ct] :cnt)
    {
        //fprintf(stderr, "[%d %d]\n", x, ct);
        if(ct == 2 && (h == 0 || dis[x] < dis[h]))
            h = x;
    }    
    int y = u;
    // cerr << "h is " << h << endl;
    // cerr << "start find Lway:" << endl;
    while(y != h)
    {
        //cerr << y << " ";
        for(int i = 0; i < L[y].size(); i++)
            if(L[y][i].first == h)
            {
                retL.push_back(make_pair(y, pred[y][i]));
                y = pred[y][i];
                break;
            }
    }
    y = v;
    // cerr << endl << "start find Rway:" << endl;
    while(y != h)
    {
        //cerr << y << " ";
        for(int i = 0; i < L[y].size(); i++)
            if(L[y][i].first == h)
            {
                retR.push_back(make_pair(y, pred[y][i]));
                y = pred[y][i];
                break;
            }
    }
    //cerr << endl;

    reverse(retR.begin(), retR.end());
    for(auto it: retR) retL.push_back(it);
    return retL;
}


vector <int> Static_HL::getuSP(int u,int v)
{
    vector <pair <int,int> > es = getSP(u, v);
    vector <int> ret;
    int now = u;
    ret.push_back(now);
    for(auto [x, y]: es)
    {
        if(now == x) now = y;
        else now = x;
        ret.push_back(now);
    }
    return ret;
}

void Static_HL::clear()
{
    n = 0;
    L.clear();
    pred.clear();
}

void Static_HL::build(Graph G)
{
    clear();
    n = G.n;
    for(int i = 0; i <= n; i++)
    {
        L.push_back(vector<pair<int, int> >());
        pred.push_back(vector <int>());
    }    

    vector <int> v;
    for(int i = 0; i <= n; i++)
        v.push_back(i);
    // random_device rd;
    // mt19937 gen(rd());
    // shuffle(v.begin(), v.end(), gen);
    // for(int i = 0; i <= n; i++)
    //     if(v[i] == 0)
    //         swap(v[i],v[0]);

    B_C betc;
    betc.build(G);
    sort(v.begin(), v.end(), [&betc](int v1, int v2) {
        return betc.BC[v1] > betc.BC[v2];
    });

    for(int i = 1; i <= n; i++)
    {
        //cout << "the " << i << "th is:" << v[i] << endl; 
        //vector <vector <pair<int,int> > > tL = L;
        vector <int> vis(n+1),d(n+1);
        for(int j = 1; j <= n; j++)
            d[j] = inf;
        d[v[i]] = 0;
        priority_queue <pair<int, int>, vector <pair<int, int> >, greater<pair<int, int> > > pq;
        pq.push(make_pair(d[v[i]], v[i]));
        vector <int> pre(n+1);
        while(!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();
            if(vis[u]) continue;
            vis[u] = 1;
            //cout << u << " is the top of pq" << endl;
            if(d[u] < getD(v[i], u))
            {
                //cout << "gone getD" << endl;
                L[u].push_back(make_pair(v[i], d[u]));
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
        //L = tL;

    }

    for(int i = 1; i <= n; i++)
        siz += L[i].size();
}

void Static_HL::debug()
{
    for(int i = 1; i <= n; i++)
    {
        printf("in %d:\n", i);
        for(int x = 0; x < L[i].size(); x++)
        {
            auto [y, w] = L[i][x];
            printf("[%d %d] = %d, par = %d\n", i, y, w, pred[i][x]);
        }
        puts("");
    }
}