struct RMQ
{
    vector<vector<ll>> sparseTable;

    RMQ() {}

    RMQ(vector<ll> &v)
    {
        ll n = v.size();
        if(n == 0) return;
        ll bitw = 64 - __builtin_clzll(n);

        sparseTable.assign(bitw, vector<ll> (n));

        for(int i = 0; i < n; i++)
        {
            sparseTable[0][i] = v[i];
        }

        for(int len = 1; len < bitw; len++)
        {
            for(int i = 0; i + (1LL<<len) <= n; i++)
            {
                sparseTable[len][i] = min(sparseTable[len-1][i], sparseTable[len-1][i + (1LL<<(len-1))]);
            }
        }
    }

    ll query(int l, int r)
    {
        int len = r - l + 1;
        int bitw = 32 - __builtin_clz(len);

        int rr = r - (1LL<<(bitw-1)) + 1;

        return min(sparseTable[bitw-1][l], sparseTable[bitw-1][rr]);
    }
};
 
struct LCA
{
    vector<ll> time;
    vector<ll> node;
    vector<ll> tour;
    vector<ll> inid, outid;
    RMQ rmq;
    ll timer = 0;

    void euler_tour(ll u, ll p, vector<vector<ll>> &adj)
    {
        time[u] = timer++;
        node[time[u]] = u;

        inid[u] = tour.size();
        tour.push_back(time[u]);

        for(auto v : adj[u])
        {
            if(v != p)
            {
                euler_tour(v,u,adj);
                tour.push_back(time[u]);
            }
        }

        outid[u] = tour.size() - 1;
    }

    LCA(vector<vector<ll>> &adj)
    {
        ll n = adj.size();
        time.resize(n);
        node.resize(n);
        inid.resize(n);
        outid.resize(n);
    
        euler_tour(0,0,adj);
        rmq = RMQ(tour);
    }

    ll query(ll u, ll v)
    {
    
        ll l = inid[u];
        ll r = inid[v];
        if(r < l)
            swap(l,r);

        return node[rmq.query(l,r)];
    }
};