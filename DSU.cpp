#include <bits/stdc++.h>
using namespace std;
#define ll long long 

struct DSU
{
    vector<ll> par,sz;

    DSU(ll n)
    {
        par.resize(n);
        sz.assign(n,1);
        iota(par.begin(), par.end(), 0LL);
    }

    ll find_par(ll u)
    {
        if(par[u] == u)   return u;
        else   return par[u] = find_par(par[u]);
    }

    bool unite(ll u, ll v)
    {
        ll x = find_par(u);
        ll y = find_par(v);

        if(x != y)
        {
            if(sz[x] < sz[y])
                swap(x,y);

            par[y] = x;
            sz[x] += sz[y];
            return true;
        }

        return false;
    }
};