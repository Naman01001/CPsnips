#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct HLD 
{
    ll n, timer;
    vector<ll> sz, head, parent, depth, pos;
    
    HLD(vector<vector<ll>> &adj, ll root = 0)
    {
        n = adj.size();
        timer = 0;
        sz.assign(n, 0);
        head.assign(n, 0);
        parent.assign(n, 0);
        depth.assign(n, 0);
        pos.assign(n, 0); // Maps node 'u' to its position in the Segment Tree
        
        dfs_sz(root, root, adj);
        dfs_hld(root, root, root, adj);
    }

    void dfs_sz(ll u, ll p, vector<vector<ll>> &adj)
    {
        sz[u] = 1;
        parent[u] = p;
        
        ll max_sub = 0;
        ll heavy_child = -1;

        for(auto v : adj[u])
        {
            if(v != p)
            {
                depth[v] = depth[u] + 1;
                dfs_sz(v, u, adj);
                sz[u] += sz[v];
                
                // Track the child with the largest subtree
                if(sz[v] > max_sub)
                {
                    max_sub = sz[v];
                    heavy_child = v;
                }
            }
        }
        
        // Swap the heavy child to the front of the adjacency list
        if(heavy_child != -1)
        {
            for(ll i = 0; i < adj[u].size(); i++)
            {
                if(adj[u][i] == heavy_child)
                {
                    swap(adj[u][0], adj[u][i]);
                    break;
                }
            }
        }
    }

    void dfs_hld(ll u, ll p, ll h, vector<vector<ll>> &adj)
    {
        head[u] = h;
        pos[u] = timer++; // Assign contiguous array position

        bool first = true;
        for(auto v : adj[u])
        {
            if(v != p)
            {
                // Because we swapped in dfs_sz, the first child we visit is ALWAYS the heavy child.
                // The heavy child inherits the current head 'h'. Light children start a new head 'v'.
                dfs_hld(v, u, first ? h : v, adj);
                first = false;
            }
        }
    }
    
    // Path Query / Path Update Logic
    ll query_path(ll u, ll v)
    {
        ll res = 0; // Set to identity (0 for sum, -INF for max, etc.)
        
        while(head[u] != head[v])
        {
            // Ensure 'v' is always the node on the lower heavy path
            if(depth[head[u]] > depth[head[v]])
            {
                swap(u, v);
            }
            
            // Process the path from v up to the head of its heavy path
            // res = combine(res, seg_tree.query(pos[head[v]], pos[v]));
            
            // Jump to the parent of the heavy path
            v = parent[head[v]]; 
        }
        
        // Both nodes are now on the SAME heavy path
        if(depth[u] > depth[v])
        {
            swap(u, v);
        }
        
        // 1. If weights are on VERTICES, include both endpoints:
        // res = combine(res, seg_tree.query(pos[u], pos[v])); 
        
        // 2. If weights are on EDGES, the LCA (which is 'u') does NOT contain path data:
        // if(u != v) res = combine(res, seg_tree.query(pos[u] + 1, pos[v]));
        
        return res;
    }
};