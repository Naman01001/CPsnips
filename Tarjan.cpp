#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct Tarjan_bridge
{
    vector<ll> intime, lotime;
    ll timer = 0;

    void add_bridge(ll u, ll v)
    {

    }

    Tarjan_bridge(vector<vector<ll>> &adj)
    {
        ll n = adj.size();
        intime.assign(n, -1);
        lotime.assign(n, -1);

        for(ll i = 0; i < n; i++)
        {
            if(intime[i] == -1)
                tarjan_dfs(i,i,adj);
        }
    }

    void tarjan_dfs(ll u, ll p, vector<vector<ll>> &adj)
    {
        intime[u] = timer++;
        lotime[u] = intime[u];

        for(auto v : adj[u])
        {
            if(v == p)  continue;

            if(intime[v] == -1)
            {
                tarjan_dfs(v,u,adj);
                lotime[u] = min(lotime[u], lotime[v]);
                
                if(lotime[v] > intime[u])
                    add_bridge(v,u);
            }
            else
            {
                lotime[u] = min(lotime[u], intime[v]);
            }
        }
    }
};

struct Tarjan_AP 
{
    vector<ll> intime, lotime;
    vector<bool> is_ap;
    ll timer = 0;

    Tarjan_AP(vector<vector<ll>> &adj) 
    {
        ll n = adj.size();
        intime.assign(n, -1);
        lotime.assign(n, -1);
        is_ap.assign(n, false);

        for (ll i = 0; i < n; i++) 
        {
            if (intime[i] == -1) {
                tarjan_dfs(i, -1, adj);
            }
        }
    }

    void tarjan_dfs(ll u, ll p, vector<vector<ll>> &adj) 
    {
        intime[u] = lotime[u] = timer++;
        ll children = 0;

        for (auto v : adj[u]) 
        {
            if (v == p) continue;

            if (intime[v] == -1) 
            {
                children++;
                tarjan_dfs(v, u, adj);
                lotime[u] = min(lotime[u], lotime[v]);

                // Condition for non-root nodes
                if (p != -1 && lotime[v] >= intime[u])
                {
                    is_ap[u] = true;
                }
            } 
            else 
            {
                lotime[u] = min(lotime[u], intime[v]);
            }
        }

        // Condition for root node
        if (p == -1 && children > 1) 
        {
            is_ap[u] = true;
        }
    }
};

struct Tarjan_BCC 
{
    vector<ll> intime, lotime, comp;
    stack<pair<ll, ll>> st;
    ll timer = 0, comp_counter = 0;

    Tarjan_BCC(vector<vector<ll>> &adj) 
    {
        ll n = adj.size();
        intime.assign(n, -1);
        lotime.assign(n, -1);
        comp.assign(n, -1); // Note: APs will be assigned to their most recently popped component

        for (ll i = 0; i < n; i++) 
        {
            if (intime[i] == -1) 
            {
                tarjan_dfs(i, -1, adj);
            }
        }
    }

    void tarjan_dfs(ll u, ll p, vector<vector<ll>> &adj) 
    {
        intime[u] = lotime[u] = timer++;
        ll children = 0;

        for (auto v : adj[u]) {
            if (v == p) continue;

            if (intime[v] == -1) {
                st.push({u, v});
                children++;
                tarjan_dfs(v, u, adj);
                lotime[u] = min(lotime[u], lotime[v]);

                // If u is an articulation point (or root), pop the current BCC
                if ((p != -1 && lotime[v] >= intime[u]) || (p == -1 && children > 0)) 
                {
                    while (true) 
                    {
                        auto edge = st.top();
                        st.pop();
                        
                        comp[edge.first] = comp_counter;
                        comp[edge.second] = comp_counter;
                        
                        if (edge.first == u && edge.second == v) break;
                    }
                    comp_counter++;
                }
            } else if (intime[v] < intime[u]) 
            {
                // Back-edge condition
                st.push({u, v});
                lotime[u] = min(lotime[u], intime[v]);
            }
        }
    }
};

struct Tarjan_SCC 
{
    vector<ll> intime, lotime, comp;
    vector<bool> in_stack;
    stack<ll> st;
    ll timer = 0, comp_counter = 0;

    Tarjan_SCC(vector<vector<ll>> &adj) 
    {
        ll n = adj.size();
        intime.assign(n, -1);
        lotime.assign(n, -1);
        comp.assign(n, -1);
        in_stack.assign(n, false);

        for (ll i = 0; i < n; i++) 
        {
            if (intime[i] == -1) 
            {
                tarjan_dfs(i, adj);
            }
        }
    }

    void tarjan_dfs(ll u, vector<vector<ll>> &adj) 
    {
        intime[u] = lotime[u] = timer++;
        st.push(u);
        in_stack[u] = true;

        for (auto v : adj[u]) 
        {
            if (intime[v] == -1) 
            {
                tarjan_dfs(v, adj);
                lotime[u] = min(lotime[u], lotime[v]);
            } 
            else if (in_stack[v]) 
            {
                // Only consider back-edges to nodes currently in the same SCC path
                lotime[u] = min(lotime[u], intime[v]);
            }
        }

        // If u is the head/root of an SCC
        if (lotime[u] == intime[u]) 
        {
            while (true) 
            {
                ll curr = st.top();
                st.pop();
                in_stack[curr] = false;
                comp[curr] = comp_counter; 
                
                if (curr == u) break;
            }
            comp_counter++;
        }
    }
};