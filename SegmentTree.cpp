#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct SegTree
{

    // Node Structure 
    // Merge Logic -> should be fast (Complexity O(N log N * M) where M is Merge time)
    // Leaf Value

    /*
        Lazy :- 
        lazy() -> Merge logic on how to stack multiple updates
        apply() -> if the update is applied how does the value change
        push() -> push updates to children
    */

    struct node
    {
        int lazy;
        int sum;
        node()
        {
            lazy = 0;
            sum = 0;
        }
    };

    vector<node> tree;

    SegTree(ll n)
    {
        tree.resize(4*(n+5));
    }

    node merge(node a, node b)
    {
        node ans;
        ans.sum = a.sum + b.sum;
        return ans;
    }

    void push(int id, int l, int r)
    {
        if(tree[id].lazy)
        {
            // Apply logic here 
            tree[id].sum += (r-l+1)*tree[id].lazy;
            tree[id].lazy = 0;
            // Push Down logic
            if(l != r)
            {
                tree[id<<1].lazy += tree[id].lazy;
                tree[id<<1 | 1].lazy += tree[id].lazy;
            }
        }
    }

    void build(int id, int l, int r, vector<ll> &v)
    {
        if(l == r)
        {
            tree[id].sum = v[l];
            tree[id].lazy = 0;
            return;
        }

        int mid = (l+r)/2;
        build(2*id, l, mid, v);
        build(2*id + 1, mid+1, r, v);
        tree[id] = merge(tree[2*id], tree[2*id + 1]);
    }

    void update(int id, int l, int r, int lq, int rq, int val)
    {
        push(id,l,r);

        if(l > rq || r < lq)
        {
            return;
        }

        if(lq <= l && r <= rq)
        {
            tree[id].lazy = val;
            push(id,l,r);
            return;
        }

        int mid = (l+r)/2;
        update(2*id, l, mid, lq, rq, val);
        update(2*id + 1, mid+1, r, lq, rq, val);
        tree[id] = merge(tree[2*id], tree[2*id + 1]);
    }

    node query(int id, int l, int r, int lq, int rq)
    {
        push(id,l,r);
        if(lq > r || rq < l)
        {
            return node();
        }

        if(lq <= l && r <= rq)
        {
            return tree[id];
        }

        int mid = (l+r)/2;
        auto q1 = query(2*id, l, mid, lq, rq);
        auto q2 = query(2*id, mid+1, r, lq, rq);
        return merge(q1,q2);
    }
};


struct PersistentSegTree 
{
    struct Node 
    {
        int lc, rc; // Left child, Right child indices
        ll sum;     // Modify this to max/min/etc based on the problem
    
        Node() 
        {
            lc = 0;
            rc = 0;
            sum = 0;
        }
    };


    int n;
    vector<Node> tree;
    vector<int> roots; // Stores the root index of every version

    // Pass the maximum possible size of the array
    // Estimated max nodes = 4*N (for build) + Q * log2(N) (for queries)
    PersistentSegTree(int n, int expected_queries = 200005) 
    {
        this->n = n;
        tree.reserve(4 * n + expected_queries * 20); 
        
        // 0-th node is a dummy/null node
        tree.push_back(Node());
    }

    int build(int l, int r, const vector<ll>& a) 
    {
        int u = tree.size();
        tree.push_back(Node());
        
        if(l == r) 
        {
            tree[u].sum = a[l];
            return u;
        }
        
        int mid = l + (r - l) / 2;
        tree[u].lc = build(l, mid, a);
        tree[u].rc = build(mid + 1, r, a);
        
        tree[u].sum = tree[tree[u].lc].sum + tree[tree[u].rc].sum;
        return u;
    }

    // Pass the root of the version you want to base this update on
    int update(int prev_u, int l, int r, int idx, ll val) 
    {
        int u = tree.size();
        tree.push_back(tree[prev_u]); // Copy previous node's state
        
        if(l == r) 
        {
            // Point update: += val or = val depending on problem
            tree[u].sum += val; 
            return u;
        }
        
        int mid = l + (r - l) / 2;
        if(idx <= mid) 
        {
            tree[u].lc = update(tree[prev_u].lc, l, mid, idx, val);
        }
        else 
        {
            tree[u].rc = update(tree[prev_u].rc, mid + 1, r, idx, val);
        }
        
        tree[u].sum = tree[tree[u].lc].sum + tree[tree[u].rc].sum;
        return u;
    }

    ll query(int u, int l, int r, int ql, int qr) 
    {
        if(u == 0 || ql > r || qr < l) 
        {
            return 0; // Out of bounds or null node
        }
        
        if(ql <= l && r <= qr) 
        {
            return tree[u].sum;
        }
        
        int mid = l + (r - l) / 2;
        return query(tree[u].lc, l, mid, ql, qr) + 
               query(tree[u].rc, mid + 1, r, ql, qr);
    }
    
    // --- Helper Functions to manage versions easily ---
    
    // Call this first to initialize version 0 from an array
    void init_version(const vector<ll>& a)
    {
        roots.push_back(build(0, n - 1, a));
    }
    
    // Applies an update to a specific old version, returning the new version's ID
    int add_version(int based_on_version, int idx, ll val)
    {
        int new_root = update(roots[based_on_version], 0, n - 1, idx, val);
        roots.push_back(new_root);
        return roots.size() - 1;
    }
    
    // Query a specific version (e.g. "What was the sum in range [ql, qr] at version v?")
    ll query_version(int version_idx, int ql, int qr)
    {
        return query(roots[version_idx], 0, n - 1, ql, qr);
    }
};