#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define ll long long

// Ordered set that stores pairs to handle duplicate values. {value, index}
typedef tree<pair<ll, ll>, null_type, less<pair<ll, ll>>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

struct FenwickOrderedSet 
{
    ll n;
    vector<ordered_set> bit;

    // 0-indexed initialization
    FenwickOrderedSet(const vector<ll>& a) 
    {
        n = a.size();
        bit.resize(n + 1); // 1-indexed internally
        for(ll i = 0; i < n; i++) 
        {
            add(i, a[i]);
        }
    }

    // Internal function to add a value to the Fenwick Tree
    void add(ll idx, ll val) 
    {
        ll bit_idx = idx + 1; 
        while(bit_idx <= n) 
        {
            bit[bit_idx].insert({val, idx});
            bit_idx += bit_idx & -bit_idx;
        }
    }

    // Internal function to remove a value from the Fenwick Tree
    void remove(ll idx, ll val) 
    {
        ll bit_idx = idx + 1;
        while(bit_idx <= n) 
        {
            bit[bit_idx].erase({val, idx});
            bit_idx += bit_idx & -bit_idx;
        }
    }

    // Point update: a[idx] changes from old_val to new_val in O(log^2 N)
    void update(ll idx, ll old_val, ll new_val) 
    {
        remove(idx, old_val);
        add(idx, new_val);
    }

    // Count elements strictly GREATER than 'val' in prefix [0...idx]
    ll query_prefix(ll idx, ll val) 
    {
        ll count = 0;
        ll bit_idx = idx + 1;
        while(bit_idx > 0) 
        {
            // Total size of this BIT node minus elements <= val
            // We use {val, 1e18} to ensure we cover all elements equal to val
            count += bit[bit_idx].size() - bit[bit_idx].order_of_key({val, 2e18});
            bit_idx -= bit_idx & -bit_idx;
        }
        return count;
    }

    // Count elements strictly GREATER than 'val' in range [l...r] in O(log^2 N)
    ll query(ll l, ll r, ll val) 
    {
        if(l > r) 
        {
            return 0;
        }
        return query_prefix(r, val) - query_prefix(l - 1, val);
    }
};