#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct LCA_BL
{
    ll n, LOG;
    vector<vector<ll>> up;
    vector<ll> depth;

    LCA_BL(vector<vector<ll>> &adj, ll root = 0)
    {
        n = adj.size();
        LOG = 0;
        
        while((1LL << LOG) <= n)
        {
            LOG++;
        }
        
        up.assign(n, vector<ll>(LOG, root));
        depth.assign(n, 0);

        dfs(root, root, adj);

        for(ll i = 1; i < LOG; i++)
        {
            for(ll j = 0; j < n; j++)
            {
                up[j][i] = up[up[j][i-1]][i-1];
            }
        }
    }

    void dfs(ll u, ll p, vector<vector<ll>> &adj)
    {
        up[u][0] = p;
        for(auto v : adj[u])
        {
            if(v != p)
            {
                depth[v] = depth[u] + 1;
                dfs(v, u, adj);
            }
        }
    }

    ll query(ll u, ll v)
    {
        if(depth[u] < depth[v])
        {
            swap(u, v);
        }

        // 1. Lift u to the same depth as v
        ll diff = depth[u] - depth[v];
        for(ll i = 0; i < LOG; i++)
        {
            if((diff >> i) & 1)
            {
                u = up[u][i];
            }
        }

        if(u == v) 
        {
            return u;
        }

        // 2. Lift both u and v together to just below the LCA
        for(ll i = LOG - 1; i >= 0; i--)
        {
            if(up[u][i] != up[v][i])
            {
                u = up[u][i];
                v = up[v][i];
            }
        }

        return up[u][0];
    }
    
    ll get_dist(ll u, ll v)
    {
        return depth[u] + depth[v] - 2 * depth[query(u, v)];
    }
};