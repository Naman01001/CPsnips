#include <bits/stdc++.h>
#define ll long long

using namespace std;

struct CentroidDecomposition
{
    vector<unordered_set<ll>> adj;
    vector<ll> size;
    vector<ll> par;

    void size_dfs(ll u, ll p)
    {
        size[u] = 1;

        for(auto v : adj[u])
        {
            if(v == p)  continue;
            size_dfs(v,u);
            size[u] += size[v];
        }
    }

    ll get_centroid(ll u, ll p, ll tsize)
    {
        for(auto v : adj[u])
        {
            if(v != p)
            {
                if(size[v] > tsize/2)
                {
                    return get_centroid(v,u,tsize);
                }
            }
        }

        return u;
    }

    void dfs(ll u, ll p)
    {
        size_dfs(u,p);
        ll tsize = size[u];
        ll cent = get_centroid(u,p,tsize);
    
        if(p != -1)
        {
            par[cent] = p;
        }
        else
        {
            par[cent] = cent;
        }

        vector<ll> childs;
        for(auto ch : adj[cent])
        {
            childs.push_back(ch);
        }

        adj[cent].clear();
        for(auto ch : childs)
        {
            adj[ch].erase(cent);
        }

        for(auto ch : childs)
        {
            dfs(ch, cent);
        }
    }

    CentroidDecomposition(vector<vector<ll>> &aadj)
    {
        ll n = aadj.size();
        adj.resize(n);
        for(int i = 0; i < n; i++)
        {
            for(auto j : aadj[i])
            {
                adj[i].insert(j);
            }
        }

        par.resize(n);
        size.resize(n);
        dfs(0,-1);
    }
};