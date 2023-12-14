#include "hbll.h"
#include <cstdio>
#include <utility>
#include <vector>
static int vid[2][1000010], hm[1000010];
void HBLL::clear()
{
    n = 0;
    L.clear();
    prew.clear();
}
void HBLL::clearbuc()
{
    memset(vid, -1, sizeof vid);
    memset(hm, -1, sizeof hm);
}
void HBLL::build_hbll(Graph G)
{
    clear();
    clearbuc();
    n = G.n;

    vector <int> v;
    for(int i = 0; i <= n; i++)
    {
        v.push_back(i);
        L.push_back(vector <Triple> ());
        prew.push_back(vector <int> ());
    }    
    B_C betc;
    betc.build(G);
    sort(v.begin(), v.end(), [&betc](int v1, int v2) {
        return betc.BC[v1] > betc.BC[v2];
    });
    
    int modlen = 10, labelsize = 0;

    for(int i = 1; i <= n; i++)
    {
        if(i >= 200) modlen = 100;
        if(i % modlen == 0)
            fprintf(stderr, "%d had done! The sum of label is %d\n", i, labelsize);
        //以 v[i] 为根， d[h][i] 带权距离 V[h] 在此层的点
        vector <int> id(n+1), V[2];
        vector <double> d[2];
        for(int j = 1; j <= n; j++) 
            id[j] = L[j].size();
        

        V[0].push_back(v[i]);
        for(int l = 0; l <= 1; l++)
            for(int j = 0; j <= n; j++)
                d[l].push_back(INF);
         
        d[0][v[i]] = 0;
        vector <int> prelas(n+1);
        prelas[v[i]] = 0;
        for(int h = 0; ; h++)
        {
            int cur = h&1;
            if(V[cur].size() == 0) break;

            sort(V[cur].begin(), V[cur].end());
            V[cur].erase(unique(V[cur].begin(), V[cur].end()), V[cur].end());

            V[cur^1].clear();
            d[cur^1] = d[cur];
            vector <int> pre(n+1);
            for(auto u: V[cur])
            {
                if(d[cur][u] >= GET_WD_build(v[i], u, h, id).first)
                    continue;
                L[u].push_back(Triple(v[i], h, d[cur][u]));
                prew[u].push_back(prelas[u]);
                ++labelsize;
                for(auto [nxt, w]: G.ed[u])
                {
                    if(d[cur][u] + w < d[cur^1][nxt])
                    {
                        V[cur^1].push_back(nxt);
                        d[cur^1][nxt] = d[cur][u] + w;
                        pre[nxt] = u;
                    }
                }
            }
            prelas = pre;
        }
    }
}
// [dis, midx, h1, h2]
pair <double ,pair <int, pair <int,int> > > HBLL::GET_WD_build(int u, int v, int h, vector <int> id = vector <int>())
{
    //fprintf(stderr, "GET_WD %d %d %d\n", u, v, h);
    int nu = L[u].size(), nv = L[v].size(); 
    if(!id.empty()) 
        nu = id[u], nv = id[v];
    pcntwd += nu + nv;
    // pot[typ][vid[typ][vv]] -> vector <pair <int,int> > which root is vv
    //map <int,int> vid[2];
    vector <vector <pair <int,double> > > pot[2];
    for(int i = 0; i <= 1; i++)
        pot[i].push_back(vector <pair <int,double> >());

    vector <int> perv[2];
    for(int i = 0; i < nu; i++)
    {
        auto it = L[u][i];
        if(it.h > h) continue;
        if(vid[0][it.l] == -1)
        {
            pot[0].push_back(vector <pair <int, double> >());
            vid[0][it.l] = pot[0].size() - 1;
            perv[0].push_back(it.l);
        }    
            
        pot[0][vid[0][it.l]].push_back(make_pair(it.h, it.d));
    }

    for(int i = 0; i < nv; i++)
    {
        auto it = L[v][i];
        if(it.h > h) continue;
        if(vid[1][it.l] == -1)
        {
            pot[1].push_back(vector <pair <int, double> >());
            vid[1][it.l] = pot[1].size() - 1;
            perv[1].push_back(it.l);
        }    
            
        pot[1][vid[1][it.l]].push_back(make_pair(it.h, it.d));
    }

    double ans = INF;
    int idl = 0, h1 = 0, h2 = 0;
    for(auto l: perv[0])
    //for(auto [l, pu]: vid[0])
    {
        int pu = vid[0][l];
        if(vid[1][l] == -1) continue;
        //if(vid[1].find(l) == vid[1].end()) continue;
        int pv = vid[1][l]; 
        auto &hdu = pot[0][pu];
        auto &hdv = pot[1][pv];
        if(hdu.empty() || hdv.empty()) continue;
        if(!id.empty())
        {
            sort(hdu.begin(), hdu.end());
            sort(hdv.begin(), hdv.end());
        }
        vector <double> miv;
        vector <int> mivid;
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

    for(int j = 0; j < 2; j++)
        for(auto x: perv[j]) vid[j][x] = -1;

    //fprintf(stderr, "ret: %d %d %d %d\n",ans, idl, h1, h2);

    return make_pair(ans, make_pair(idl, make_pair(h1, h2)));
}

int HBLL::GET_UD(int u, int v)
{
    //fprintf(stderr, "GET_UD\n");
    ++cntud;
    pcntud += L[u].size() + L[v].size();
    //map <int, int > hm;
    vector <int> perv;
    for(auto it: L[u])
    {
        hm[it.l] = it.h;
        perv.push_back(it.l);
    }    
    int ans = inf;
    for(auto it: L[v])
        if(hm[it.l] != -1) //if(hm.find(it.l) != hm.end())
            ans = min(ans, hm[it.l] + it.h);
    for(auto x: perv) hm[x] = -1;
    //fprintf(stderr, "%d %d %d\n", u, v, ans);
    return ans;
}

double HBLL::GET_WD(int u, int v, int h)
{
    ++cntwd;
    return GET_WD_build(u, v, h).first;
}

vector <int> HBLL::GET_MWP(int u, int v, int h)
{
    auto info = GET_WD_build(u, v, h).second;
    int idl = info.first;
    auto [h1, h2] = info.second;
    int y = u;
    vector <int> retL, retR;
    //fprintf(stderr, "u, v, h = [%d, %d, %d], idl, h1, h2 = [%d, %d, %d]\n", u, v, h, idl, h1, h2);
    while(y != idl)
    {
        //fprintf(stderr, "%d ", y);
        for(int i = 0; i < L[y].size(); i++)
            if(L[y][i].l == idl && L[y][i].h == h1)
            {
                retL.push_back(y);
                y = prew[y][i];
                --h1;
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
                y = prew[y][i];
                --h2;
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
            printf("[%d, %d, %.6lf]\n", it.l, it.h, it.d);
        puts("");
    }
}

void HBLL::output_F(string filepre)
{
    ofstream outputFile(filepre+"/hbll.txt");

    for(int i = 1; i <= n; i++)
    {
        for(int j = 0; j < L[i].size(); j++)
            outputFile << L[i][j].l << " " << L[i][j].h << " " << L[i][j].d << " " << prew[i][j] << ",";
        outputFile << endl;
    }

    outputFile.close();
}

double HBLL::Average_L()
{
    int sum = 0;
    for(int i = 1; i <= n; i++) sum+=L[i].size();
    return 1.0*sum/(1.0*n);
}

void HBLL::clearcnt()
{
    cntwd = cntud = 0;
    pcntwd = pcntud = 0;
}

void HBLL::printcnt()
{
    fprintf(stderr, "ud:[%d %lld], wd:[%d %lld]\n",cntud, pcntud, cntwd, pcntwd);
}

void HBLL::rearrange()
{
    for(int i = 1; i <= n; i++)
    {
        int len = L[i].size();
        //fprintf(stderr, "%d %d\n",i, len);
        vector <int> Id;
        for(int j = 0; j <= len - 1; j++) Id.push_back(j);
        sort(Id.begin(), Id.end(), [&](int x,int y){
            if(L[i][x].l==L[i][y].l)
                return L[i][x].h == L[i][y].h? L[i][x].d < L[i][y].d : L[i][x].h < L[i][y].h;
            return L[i][x].l < L[i][y].l;
        });
        vector <Triple> tmpl = L[i];
        vector <int> tmpp = prew[i];
        for(int j = 0; j <= len - 1; j++) 
        {
            tmpl[j] = L[i][Id[j]];
            tmpp[j] = prew[i][Id[j]];
        }    
        L[i] = tmpl;
        prew[i] = tmpp;
    }
}