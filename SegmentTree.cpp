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

    void build(int id, int l, int r, vl &v)
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