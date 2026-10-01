#include <bits/stdc++.h>
using namespace std;
#define ll long long

vector<ll> toposort(vector<vector<ll>> &adj)
{
    ll n = adj.size();
    vector<ll> indeg(n,0);

    for(ll i = 0; i < n; i++)
    {
        for(auto j : adj[i])
        {
            indeg[j]++;
        }
    }

    queue<ll> q;
    vector<ll> order;

    for(ll i = 0; i < n; i++)
    {
        if(indeg[i] == 0)
            q.push(i);
    }

    while(q.size())
    {
        ll u = q.front(); q.pop();
        order.push_back(u);
        for(auto v : adj[u])
        {
            indeg[v]--;
            if(indeg[v] == 0)
            {
                q.push(v);
            }
        }
    }

    return order;
}